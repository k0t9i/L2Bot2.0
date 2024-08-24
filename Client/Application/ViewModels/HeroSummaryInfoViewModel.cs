using Client.Domain.AI;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
        public string AIType
        {
            get
            {
                return ai.Type.ToString();
            }
        }
        public string AIState
        {
            get
            {
                return ai.IsEnabled ? ai.CurrentState.ToString() : "Disabled";
            }
        }
        public int InventoryOccupiedSlots
        {
            get
            {
                return items.Count + questItems.Count;
            }
        }
        public HeroSummaryInfoViewModel(Hero hero, AIInterface ai, ObservableCollection<ItemListViewModel> items, ObservableCollection<ItemListViewModel> questItems)
        {
            this.hero = hero;
            this.ai = ai;
            this.items = items;
            this.questItems = questItems;
            hero.FullName.PropertyChanged += FullName_PropertyChanged;
            hero.Phenotype.PropertyChanged += Phenotype_PropertyChanged;
            hero.ExperienceInfo.PropertyChanged += ExperienceInfo_PropertyChanged;
            hero.Transform.PropertyChanged += Transform_PropertyChanged;
            hero.VitalStats.PropertyChanged += VitalStats_PropertyChanged;
            hero.InventoryInfo.PropertyChanged += InventoryInfo_PropertyChanged;
            hero.PropertyChanged += Hero_PropertyChanged;
            ai.PropertyChanged += Ai_PropertyChanged;
            items.CollectionChanged += Items_CollectionChanged;
            questItems.CollectionChanged += QuestItems_CollectionChanged;
        }

        private void QuestItems_CollectionChanged(object? sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            OnPropertyChanged("InventoryOccupiedSlots");
        }

        private void Items_CollectionChanged(object? sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            OnPropertyChanged("InventoryOccupiedSlots");
        }

        private void Ai_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Type")
            {
                OnPropertyChanged("AIType");
            }
            if (e.PropertyName == "CurrentState" || e.PropertyName == "IsEnabled")
            {
                OnPropertyChanged("AIState");
            }
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
        private readonly AIInterface ai;
        private readonly ObservableCollection<ItemListViewModel> items;
        private readonly ObservableCollection<ItemListViewModel> questItems;
        private TargetSummaryInfoViewModel? target;
    }
}
