using Client.Domain.Common;
using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class CreatureListViewModel : ObservableObject
    {
        public uint Id => creature.Id;

        public string Name => creature.Name;

        public string BriefInfo => creature.BriefInfo;

        public float Distance => creature.Transform.Position.HorizontalDistance(hero.Transform.Position) / 100;

        public float DeltaZ => (creature.Transform.Position.Z - hero.Transform.Position.Z) / 100;

        public CreatureListViewModel(CreatureInterface creature, Hero hero)
        {
            creature.PropertyChanged += Creature_PropertyChanged;
            creature.Transform.Position.PropertyChanged += Position_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;

            this.creature = creature;
            this.hero = hero;
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
    }
}
