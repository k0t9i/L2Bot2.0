using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows;
using Client.Application.Commands;
using System.Net.Http;

namespace Client.Application.ViewModels
{
    public class CreatureMapViewModel : ObservableObject
    {
        public void MapUpdated(float scale, float viewportWidth, float viewportHeight)
        {
            Scale = scale;
            VieportSize = new Vector3(viewportWidth, viewportHeight, 0);
        }

        public uint Id => creature.Id;
        public string Name => creature.Name;
        public Vector3 Position => new Vector3(
            (creature.Transform.Position.X - hero.Transform.Position.X) / scale + (VieportSize.X / 2),
            (creature.Transform.Position.Y - hero.Transform.Position.Y) / scale + (VieportSize.Y / 2),
            0
        );
        public Vector3 Direction => new Vector3(
            creature.Transform.Direction.X * Radius * 2f,
            creature.Transform.Direction.Y * Radius * 2f,
            0
        );
        public VitalStats VitalStats => creature.VitalStats;
        public float Radius => MAX_RADIUS - (1/MapViewModel.MIN_SCALE - 1/scale) / (1/MapViewModel.MIN_SCALE - 1/MapViewModel.MAX_SCALE) * (MAX_RADIUS - MIN_RADIUS);
        public float Scale
        {
            get => scale;
            set
            {
                if (scale != value)
                {
                    scale = value;
                    OnPropertyChanged("Position");
                    OnPropertyChanged("Direction");
                    OnPropertyChanged("Radius");
                    OnPropertyChanged("AggroRadius");
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
                    OnPropertyChanged("Direction");
                }
            }
        }
        public CreatureTypeEnum Type => creature.Type;
        public bool IsTarget => Id == hero.TargetId;
        public bool IsAggressive => creature.AggroRadius > 0 && !creature.VitalStats.IsDead && creature.IsHostile;
        public float AggroRadius => creature.AggroRadius / scale;
        public bool IsAttacker => hero.AttackerIds.Contains(creature.Id);
        public bool IsDead => creature.VitalStats.IsDead;
        public bool IsHostile => creature.IsHostile;
        public bool IsSweepable => creature is NPC && ((NPC) creature).SpoilState == SpoilStateEnum.Sweepable;

        public ICommand MouseLeftClickCommand { get; }
        public ICommand MouseLeftDoubleClickCommand { get; }
        public ICommand MouseRightClickCommand { get; }
        private void OnMouseLeftClick(object? obj)
        {
            worldHandler.RequestAcquireTarget(Id);
        }

        private void OnMouseLeftDoubleClick(object? obj)
        {
            worldHandler.RequestAttackOrFollow(Id);
        }
        
        private async Task OnMouseRightClick(object? obj)
        {
            await pathMover.MoveUntilReachedAsync(creature.Transform.Position);
        }

        public CreatureMapViewModel(WorldHandler worldHandler, AsyncPathMoverInterface pathMover, CreatureInterface creature, Hero hero)
        {
            this.creature = creature;
            this.hero = hero;
            this.worldHandler = worldHandler;
            this.pathMover = pathMover;
            creature.PropertyChanged += Creature_PropertyChanged;
            creature.Transform.PropertyChanged += Transform_PropertyChanged;
            creature.Transform.Position.PropertyChanged += Position_PropertyChanged;
            creature.VitalStats.PropertyChanged += VitalStats_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
            hero.PropertyChanged += Hero_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnMouseLeftClick);
            MouseLeftDoubleClickCommand = new RelayCommand(OnMouseLeftDoubleClick);
            MouseRightClickCommand = new RelayCommand(async (o) => await OnMouseRightClick(o));
        }

        private void VitalStats_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Hp" || e.PropertyName == "MaxHp")
            {
                OnPropertyChanged("VitalStats");
            }
            if (e.PropertyName == "IsDead")
            {
                OnPropertyChanged("IsAggressive");
                OnPropertyChanged("IsDead");
            }
        }

        private void Hero_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "TargetId")
            {
                OnPropertyChanged("IsTarget");
            }
            if (e.PropertyName == "AttackerIds")
            {
                OnPropertyChanged("IsAttacker");
            }
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Position");
            OnPropertyChanged("Direction");
        }

        private void Position_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Position");
            OnPropertyChanged("Direction");
        }

        private void Transform_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Direction")
            {
                OnPropertyChanged("Direction");
            }
        }

        private void Creature_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Name")
            {
                OnPropertyChanged("Name");
            }
            if (e.PropertyName == "SpoilState")
            {
                OnPropertyChanged("IsSweepable");
            }
        }

        private readonly CreatureInterface creature;
        private readonly Hero hero;
        private readonly WorldHandler worldHandler;
        private readonly AsyncPathMoverInterface pathMover;
        private float scale = 1;
        private static readonly float MAX_RADIUS = 10;
        private static readonly float MIN_RADIUS = 2;
        private Vector3 vieportSize = new Vector3(0, 0, 0);
    }
}
