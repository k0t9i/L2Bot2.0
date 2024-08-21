﻿using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class HeroSummaryInfoViewModel : ObservableObject
    {
        public bool IsVisible {
            get
            {
                return true;
            }
        }
        public FullName Fullname
        {
            get
            {
                return hero.FullName;
            }
        }
        public Phenotype Phenotype
        {
            get
            {
                return hero.Phenotype;
            }
        }
        public Transform Transform
        {
            get
            {
                return hero.Transform;
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
        public InventoryInfo InventoryInfo
        {
            get
            {
                return hero.InventoryInfo;
            }
        }
        public ulong Money
        {
            get
            {
                return 0;
            }
        }

        public TargetSummaryInfoViewModel? Target
        {
            get
            {
                return target;
            }
        }
        public List<uint> Attackers
        {
            get
            {
                return hero.AttackerIds.ToList();
            }
        }
        public HeroSummaryInfoViewModel(Hero hero)
        {
            this.hero = hero;

            hero.FullName.PropertyChanged += FullName_PropertyChanged;
            hero.Phenotype.PropertyChanged += Phenotype_PropertyChanged;
            hero.ExperienceInfo.PropertyChanged += ExperienceInfo_PropertyChanged;
            hero.Transform.PropertyChanged += Transform_PropertyChanged;
            hero.VitalStats.PropertyChanged += VitalStats_PropertyChanged;
            hero.InventoryInfo.PropertyChanged += InventoryInfo_PropertyChanged;
            hero.PropertyChanged += Hero_PropertyChanged;
        }

        private void Hero_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Target")
            {
                if (target == null && hero.Target != null)
                {
                    target = new TargetSummaryInfoViewModel(hero.Target, hero);
                    OnPropertyChanged("Target");
                }
                else if (target != null && hero.Target == null)
                {
                    target.UnsubscribeAll();
                    target = null;
                    OnPropertyChanged("Target");
                }
            }
            else if (e.PropertyName == "AttackerIds")
            {
                OnPropertyChanged("Attackers");
            }
        }

        private void InventoryInfo_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("InventoryInfo");
        }

        private void VitalStats_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("VitalStats");
        }

        private void Transform_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Transform");
        }

        private void Phenotype_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Phenotype");
        }

        private void ExperienceInfo_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Experience");
        }

        private void FullName_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Fullname");
        }

        private readonly Hero hero;
        private TargetSummaryInfoViewModel? target;
    }
}
