using Client.Application.Commands;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Client.Application.ViewModels
{
    public class CreatureListViewModel : ObservableObject
    {
        public uint Id => creature.Id;

        public string Name => creature.Name;

        public string BriefInfo => creature.BriefInfo;

        public float Distance => creature.Distance(hero);

        public float DeltaZ => creature.DeltaZ(hero);

        public bool IsTarget => Id == hero.TargetId;

        public bool IsAttacker => hero.AttackerIds.Contains(creature.Id);

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

        public CreatureListViewModel(WorldHandler worldHandler, AsyncPathMoverInterface pathMover, CreatureInterface creature, Hero hero)
        {
            creature.PropertyChanged += Creature_PropertyChanged;
            creature.Transform.Position.PropertyChanged += Position_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
            hero.PropertyChanged += Hero_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnMouseLeftClick);
            MouseLeftDoubleClickCommand = new RelayCommand(OnMouseLeftDoubleClick);
            MouseRightClickCommand = new RelayCommand(async (o) => await OnMouseRightClick(o));

            this.creature = creature;
            this.hero = hero;
            this.worldHandler = worldHandler;
            this.pathMover = pathMover;
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Distance");
            OnPropertyChanged("DeltaZ");
        }

        private void Position_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Distance");
            OnPropertyChanged("DeltaZ");
        }

        private void Creature_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Name")
            {
                OnPropertyChanged("Name");
            }
            if (e.PropertyName == "BriefInfo")
            {
                OnPropertyChanged("BriefInfo");
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

        private readonly CreatureInterface creature;
        private readonly Hero hero;
        private readonly WorldHandler worldHandler;
        private readonly AsyncPathMoverInterface pathMover;
    }
}
