﻿using Client.Application.Commands;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Client.Application.ViewModels
{
    public class DropListViewModel : ObservableObject
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
                return drop.Transform.Position.HorizontalDistance(hero.Transform.Position);
            }
        }
        public float DeltaZ
        {
            get
            {
                return (drop.Transform.Position.Z - hero.Transform.Position.Z);
            }
        }

        public string IconName
        {
            get
            {
                return "/Assets/icons/" + drop.IconName + ".png";
            }
        }

        public ICommand MouseLeftClickCommand { get; }
        public ICommand MouseRightClickCommand { get; }
        private void OnMouseLeftClick(object? obj)
        {
            worldHandler.RequestPickUp(Id);
        }
        private async Task OnMouseRightClick(object? obj)
        {
            await pathMover.MoveUntilReachedAsync(drop.Transform.Position);
        }

        public DropListViewModel(WorldHandler worldHandler, AsyncPathMoverInterface pathMover, Drop drop, Hero hero)
        {
            this.drop = drop;
            this.hero = hero;
            this.worldHandler = worldHandler;
            this.pathMover = pathMover;
            drop.PropertyChanged += Drop_PropertyChanged;
            drop.Transform.Position.PropertyChanged += DropPosition_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnMouseLeftClick);
            MouseRightClickCommand = new RelayCommand(async (o) => await OnMouseRightClick(o));
        }

        public void UnsubscribeAll()
        {
            drop.PropertyChanged -= Drop_PropertyChanged;
            drop.Transform.Position.PropertyChanged -= DropPosition_PropertyChanged;
            hero.Transform.Position.PropertyChanged -= HeroPosition_PropertyChanged;
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
        private readonly WorldHandler worldHandler;
        private readonly AsyncPathMoverInterface pathMover;
    }
}
