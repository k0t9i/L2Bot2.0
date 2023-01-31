using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class TargetSummaryInfoViewModel : ObservableObject
    {
        public uint Id => creature.Id;

        public string Name => creature.Name;
        
        public Vector3 Position => creature.Transform.Position;

        public string BriefInfo => creature.BriefInfo;

        public float Distance => creature.Distance(hero);

        public float DeltaZ => creature.DeltaZ(hero);

        public uint Hp => creature.VitalStats.Hp;

        public uint MaxHp => creature.VitalStats.MaxHp;

        public TargetSummaryInfoViewModel(CreatureInterface creature, Hero hero)
        {
            creature.PropertyChanged += Creature_PropertyChanged;
            creature.Transform.Position.PropertyChanged += Position_PropertyChanged;
            creature.VitalStats.PropertyChanged += VitalStats_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;

            this.creature = creature;
            this.hero = hero;
        }

        private void VitalStats_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Hp")
            {
                OnPropertyChanged("Hp");
            }
            if (e.PropertyName == "MaxHp")
            {
                OnPropertyChanged("MaxHp");
            }
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
            OnPropertyChanged("Transform");
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
