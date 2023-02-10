using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using Client.Domain.Service;
using System.Windows.Input;
using Client.Application.Commands;

namespace Client.Application.ViewModels
{
    public class DropMapViewModel : ObservableObject
    {
        public uint Id => drop.Id;
        public string Name => drop.Name + " (" + drop.Amount + ")";
        public Vector3 Position => new Vector3(
            (drop.Transform.Position.X - hero.Transform.Position.X) / scale + (VieportSize.X / 2),
            (drop.Transform.Position.Y - hero.Transform.Position.Y) / scale + (VieportSize.Y / 2),
            0
        );
        public float Radius => MathF.Max(MAX_RADIUS / scale, MIN_RADIUS);
        public float Scale
        {
            get => scale;
            set
            {
                if (scale != value)
                {
                    scale = value;
                    OnPropertyChanged("Position");
                    OnPropertyChanged("Radius");
                }
            }
        }
        public Vector3 VieportSize
        {
            get => vieportSize;
            set
            {
                if (vieportSize != value)
                {
                    vieportSize = value;
                    OnPropertyChanged("Position");
                }
            }
        }

        public ICommand MouseLeftClickCommand { get; }
        public ICommand MouseRightClickCommand { get; }
        private void OnMouseLeftClick(object? obj)
        {
            worldHandler.RequestPickUp(Id);
        }
        private void OnMouseRightClick(object? obj)
        {
            worldHandler.RequestMoveToEntity(Id);
        }

        public DropMapViewModel(WorldHandler worldHandler, Drop drop, Hero hero)
        {
            this.drop = drop;
            this.hero = hero;
            this.worldHandler = worldHandler;
            drop.PropertyChanged += Creature_PropertyChanged;
            drop.Transform.Position.PropertyChanged += Position_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnMouseLeftClick);
            MouseRightClickCommand = new RelayCommand(OnMouseRightClick);
        }

        private void HeroPosition_PropertyChanged(object? sender, PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Position");
        }

        private void Position_PropertyChanged(object? sender, PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Position");
        }

        private void Creature_PropertyChanged(object? sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Name" || e.PropertyName == "Amount")
            {
                OnPropertyChanged("Name");
            }
        }

        private readonly Drop drop;
        private readonly Hero hero;
        private readonly WorldHandler worldHandler;
        private float scale = 1;
        private static readonly float MAX_RADIUS = 8;
        private static readonly float MIN_RADIUS = 2;
        private Vector3 vieportSize = new Vector3(0, 0, 0);
    }
}
