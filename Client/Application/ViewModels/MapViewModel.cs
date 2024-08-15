using Client.Application.Commands;
using Client.Domain.Common;
using Client.Domain.DTO;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;

namespace Client.Application.ViewModels
{
    public class MapViewModel : ObservableObject
    {
        public delegate void MapUpdatedEventHandler(float scale, float viewportWidth, float viewportHeight);
        private event MapUpdatedEventHandler? MapUpdated;

        public Hero? Hero
        {
            get => hero;
            set
            {
                if (hero != value)
                {
                    if (hero != null)
                    {
                        hero.Transform.Position.PropertyChanged -= HeroPosition_PropertyChanged;
                    }
                    hero = value;
                    if (hero != null)
                    {
                        hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
                    }
                    UpdateMap();
                }
            }
        }

        public double ViewportWidth
        {
            get => viewportWidth;
            set
            {
                if (viewportWidth != value)
                {
                    viewportWidth = value;
                    UpdateMap();
                    OnPropertyChanged("MousePosition");
                }
            }
        }
        public double ViewportHeight
        {
            get => viewportHeight;
            set
            {
                if (viewportHeight != value)
                {
                    viewportHeight = value;
                    UpdateMap();
                    OnPropertyChanged("MousePosition");
                }
            }
        }

        public float Scale
        {
            get => scale;
            set
            {
                if (scale != value)
                {
                    scale = value;
                    UpdateMap();
                    OnPropertyChanged();
                    OnPropertyChanged("MousePosition");
                }
            }
        }

        public Vector3 MousePosition
        {
            get => mousePosition;
            set => mousePosition = value;
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            UpdateMap();
            if (e.PropertyName == "X" || e.PropertyName == "Y")
            {
                OnPropertyChanged("MousePosition");
            }
        }

        private void UpdateMap()
        {
            foreach (var block in Blocks)
            {
                block.Visible = false;
            }

            if (hero != null)
            {
                var blocks = selector.SelectImages((float)ViewportWidth, (float)ViewportHeight, hero.Transform.Position, Scale);

                foreach (var block in blocks)
                {
                    if (this.blocks.ContainsKey(block.Id))
                    {
                        this.blocks[block.Id].MapBlock.DeltaX = block.DeltaX;
                        this.blocks[block.Id].MapBlock.DeltaY = block.DeltaY;
                        this.blocks[block.Id].MapBlock.Size = block.Size;

                    }
                    else
                    {
                        var model = new MapBlockViewModel(block);
                        this.blocks.Add(block.Id, model);
                        Blocks.Add(model);
                    }

                    this.blocks[block.Id].Visible = true;
                }

                
                if (MapUpdated != null)
                {
                    MapUpdated(scale, (float)ViewportWidth, (float)ViewportHeight);
                }
            }
        }

        public ICommand MouseLeftClickCommand { get; }
        private async Task OnLeftMouseClick(object? obj)
        {
            if (obj == null)
            {
                return;
            }
            if (hero == null)
            {
                return;
            }

            Point mousePos = Mouse.GetPosition((IInputElement)obj);
            var location = new Vector3(
                (float)(mousePos.X - ViewportWidth / 2) * scale + hero.Transform.Position.X,
                (float)(mousePos.Y - ViewportHeight / 2) * scale + hero.Transform.Position.Y,
                hero.Transform.Position.Z
            );

            await pathMover.MoveUntilReachedAsync(location);
        }

        public void OnMouseWheel(object sender, MouseWheelEventArgs e)
        {
            var newScale = Scale - e.Delta / Mouse.MouseWheelDeltaForOneLine;
            Scale = MathF.Max(MathF.Min(newScale, MAX_SCALE), MIN_SCALE);
        }

        public void OnMouseLeave(object sender, MouseEventArgs e)
        {
            mousePosition.X = 0;
            mousePosition.Y = 0;
        }

        public void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (hero == null)
            {
                mousePosition.X = 0;
                mousePosition.Y = 0;
                return;
            }
            var el = (IInputElement)sender;
            var mousePos = e.GetPosition(el);

            mousePosition.X = (float)(mousePos.X - ViewportWidth / 2) * scale + hero.Transform.Position.X;
            mousePosition.Y = (float)(mousePos.Y - ViewportHeight / 2) * scale + hero.Transform.Position.Y;
        }

        public MapViewModel(AsyncPathMoverInterface pathMover)
        {
            Creatures.CollectionChanged += Creatures_CollectionChanged;
            Drops.CollectionChanged += Drops_CollectionChanged;
            Path.CollectionChanged += Path_CollectionChanged;
            MouseLeftClickCommand = new RelayCommand(async (o) => await OnLeftMouseClick(o));
            mousePosition.PropertyChanged += MousePosition_PropertyChanged;
            BindingOperations.EnableCollectionSynchronization(Path, pathCollectionLock);
            this.pathMover = pathMover;
            this.pathMover.Path.CollectionChanged += PathMover_Path_CollectionChanged;
        }

        private void PathMover_Path_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            lock(pathCollectionLock)
            {
                if (e.Action == NotifyCollectionChangedAction.Add && e.NewItems != null)
                {
                    if (hero != null)
                    {
                        foreach (var item in e.NewItems)
                        {
                            var node = (PathSegment)item;
                            Path.Add(new PathNodeViewModel(node.From, node.To, hero));
                        }
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Remove && e.OldItems != null)
                {
                    foreach (var item in e.OldItems)
                    {
                        Path.RemoveAt(0);
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Reset)
                {
                    Path.RemoveAll();
                }
            }
        }

        private void Path_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add && e.NewItems != null)
            {
                foreach (var item in e.NewItems)
                {
                    var node = (PathNodeViewModel)item;
                    MapUpdated += node.MapUpdated;
                    node.MapUpdated(scale, (float)ViewportWidth, (float)ViewportHeight);
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove && e.OldItems != null)
            {
                foreach (var item in e.OldItems)
                {
                    var node = (PathNodeViewModel)item;
                    MapUpdated -= node.MapUpdated;
                }
            }
        }

        private void MousePosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("MousePosition");
        }

        private void Drops_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add && e.NewItems != null)
            {
                foreach (var item in e.NewItems)
                {
                    var drop = (DropMapViewModel)item;
                    MapUpdated += drop.MapUpdated;
                    drop.MapUpdated(scale, (float)ViewportWidth, (float)ViewportHeight);
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove && e.OldItems != null)
            {
                foreach (var item in e.OldItems)
                {
                    var drop = (DropMapViewModel)item;
                    MapUpdated -= drop.MapUpdated;
                }
            }
        }

        private void Creatures_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add && e.NewItems != null)
            {
                foreach (var item in e.NewItems)
                {
                    var creature = (CreatureMapViewModel)item;
                    MapUpdated += creature.MapUpdated;
                    creature.MapUpdated(scale, (float)ViewportWidth, (float)ViewportHeight);
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove && e.OldItems != null)
            {
                foreach (var item in e.OldItems)
                {
                    var creature = (CreatureMapViewModel)item;
                    MapUpdated -= creature.MapUpdated;
                }
            }
        }

        public ObservableCollection<MapBlockViewModel> Blocks { get; } = new ObservableCollection<MapBlockViewModel>();
        public ObservableCollection<CreatureMapViewModel> Creatures { get; } = new ObservableCollection<CreatureMapViewModel>();
        public ObservableCollection<DropMapViewModel> Drops { get; } = new ObservableCollection<DropMapViewModel>();
        public ObservableCollection<PathNodeViewModel> Path { get; } = new ObservableCollection<PathNodeViewModel>();
        public AICombatZoneMapViewModel? CombatZone {
            get
            {
                return combatZone;
            }
            set
            {
                if (value != combatZone)
                {
                    if (value == null)
                    {
                        if (combatZone != null)
                        {
                            MapUpdated -= combatZone.MapUpdated;
                        }
                    }
                    else
                    {
                        MapUpdated += value.MapUpdated;
                    }
                    combatZone = value;
                    OnPropertyChanged();
                }
            }
        }

        public readonly static float MIN_SCALE = 1;
        public readonly static float MAX_SCALE = 128;
        private readonly AsyncPathMoverInterface pathMover;
        private MapImageSelector selector = new MapImageSelector();
        private Dictionary<uint, MapBlockViewModel> blocks = new Dictionary<uint, MapBlockViewModel>();
        private Hero? hero;
        private float scale = 8;
        private double viewportWidth = 0;
        private double viewportHeight = 0;
        private Vector3 mousePosition = new Vector3(0, 0, 0);
        private object pathCollectionLock = new object();
        private AICombatZoneMapViewModel? combatZone = null;
    }
}
