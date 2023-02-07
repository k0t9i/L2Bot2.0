using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media.Animation;
using System.Collections.Specialized;

namespace Client.Application.ViewModels
{
    public class MapViewModel : ObservableObject
    {
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
                }
            }
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            UpdateMap();
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

                foreach (var creature in Creatures)
                {
                    creature.Scale = scale;
                    creature.VieportSize = new Vector3((float)ViewportWidth, (float)ViewportHeight, 0);
                }

                foreach (var drop in Drops)
                {
                    drop.Scale = scale;
                    drop.VieportSize = new Vector3((float)ViewportWidth, (float)ViewportHeight, 0);
                }
            }
        }

        public MapViewModel()
        {
            Creatures.CollectionChanged += Creatures_CollectionChanged;
            Drops.CollectionChanged += Drops_CollectionChanged;
        }

        private void Drops_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add && e.NewItems != null)
            {
                foreach (var item in e.NewItems)
                {
                    var creature = (DropMapViewModel)item;
                    creature.Scale = scale;
                    creature.VieportSize = new Vector3((float)ViewportWidth, (float)ViewportHeight, 0);
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
                    creature.Scale = scale;
                    creature.VieportSize = new Vector3((float)ViewportWidth, (float)ViewportHeight, 0);
                }
            }
        }

        public ObservableCollection<MapBlockViewModel> Blocks { get; } = new ObservableCollection<MapBlockViewModel>();
        public ObservableCollection<CreatureMapViewModel> Creatures { get; } = new ObservableCollection<CreatureMapViewModel>();
        public ObservableCollection<DropMapViewModel> Drops { get; } = new ObservableCollection<DropMapViewModel>();

        private MapImageSelector selector = new MapImageSelector();
        private Dictionary<uint, MapBlockViewModel> blocks = new Dictionary<uint, MapBlockViewModel>();
        private Hero? hero;
        private float scale = 8;
        private double viewportWidth = 0;
        private double viewportHeight = 0;
    }
}
