using Client.Domain.Common;
using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class DropListViewModel : NotifyPropertyChanged
    {
        public uint Id
        {
            get
            {
                return drop.Id;
            }
        }

        public string Name
        {
            get
            {
                return drop.Name;
            }
        }

        public uint Amount
        {
            get
            {
                return drop.Amount;
            }
        }

        public uint ItemId
        {
            get
            {
                return drop.ItemId;
            }
        }

        public float Distance
        {
            get
            {
                return drop.Transform.Position.HorizontalDistance(hero.Transform.Position) / 100;
            }
        }
        public float DeltaZ
        {
            get
            {
                return (drop.Transform.Position.Z - hero.Transform.Position.Z) / 100;
            }
        }

        public string IconName
        {
            get
            {
                return "/Assets/icons/" + drop.IconName + ".png";
            }
        }

        public DropListViewModel(Drop drop, Hero hero)
        {
            this.drop = drop;
            this.hero = hero;

            drop.PropertyChanged += Drop_PropertyChanged;
            drop.Transform.Position.PropertyChanged += DropPosition_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Distance");
            OnPropertyChanged("DeltaZ");
        }

        private void DropPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Distance");
            OnPropertyChanged("DeltaZ");
        }

        private void Drop_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Name")
            {
                OnPropertyChanged("Name");
            }
            if (e.PropertyName == "Amount")
            {
                OnPropertyChanged("Amount");
            }
            if (e.PropertyName == "ItemId")
            {
                OnPropertyChanged("ItemId");
            }
            if (e.PropertyName == "IconName")
            {
                OnPropertyChanged("IconName");
            }
        }

        private readonly Drop drop;
        private readonly Hero hero;
    }
}
