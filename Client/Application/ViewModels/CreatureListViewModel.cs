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

        private void OnMouseRightClick(object? obj)
        {
            worldHandler.RequestMoveToEntity(Id);
        }

        public CreatureListViewModel(CreatureInterface creature, Hero hero, WorldHandler worldHandler)
        {
            creature.PropertyChanged += Creature_PropertyChanged;
            creature.Transform.Position.PropertyChanged += Position_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnMouseLeftClick);
            MouseLeftDoubleClickCommand = new RelayCommand(OnMouseLeftDoubleClick);
            MouseRightClickCommand = new RelayCommand(OnMouseRightClick);

            this.creature = creature;
            this.hero = hero;
            this.worldHandler = worldHandler;
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

        private readonly CreatureInterface creature;
        private readonly Hero hero;
        private readonly WorldHandler worldHandler;
    }
}
