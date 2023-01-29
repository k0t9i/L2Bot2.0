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
    public class HeroSummaryInfoViewModel : NotifyPropertyChanged
    {
        public string Fullname
        {
            get
            {
                return hero.FullName.Nickname;
            }
        }
        public string Race
        {
            get
            {
                //todo race string
                return hero.Phenotype.Race.ToString();
            }
        }
        public string Class
        {
            get
            {
                //todo class string
                return hero.Phenotype.Class.ToString();
            }
        }
        public Vector3 Position
        {
            get
            {
                return hero.Transform.Position;
            }
        }
        public ExperienceInfo Experience
        {
            get
            {
                return hero.ExperienceInfo;
            }
        }
        public VitalStats VitalStats
        {
            get
            {
                return hero.VitalStats;
            }
        }
        public HeroSummaryInfoViewModel(Hero hero)
        {
            this.hero = hero;

            hero.FullName.PropertyChanged += FullName_PropertyChanged;
            hero.Phenotype.PropertyChanged += Phenotype_PropertyChanged;
            hero.ExperienceInfo.PropertyChanged += ExperienceInfo_PropertyChanged;
            hero.Transform.Position.PropertyChanged += Position_PropertyChanged;
            hero.VitalStats.PropertyChanged += VitalStats_PropertyChanged;
        }

        private void VitalStats_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("VitalStats");
        }

        private void Position_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Position");
        }

        private void Phenotype_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Class")
            {
                OnPropertyChanged("Class");
            }
            if (e.PropertyName == "Race")
            {
                OnPropertyChanged("Race");
            }
        }

        private void ExperienceInfo_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Exp" || e.PropertyName == "ExpToLevel" || e.PropertyName == "Level")
            {
                OnPropertyChanged("Experience");
            }
        }

        private void FullName_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Nickname")
            {
                OnPropertyChanged("Fullname");
            }
        }

        private readonly Hero hero;
    }
}
