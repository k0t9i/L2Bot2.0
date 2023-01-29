using Client.Domain.Common;
using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class PlayerListViewModel : NotifyPropertyChanged, CreatureListViewModelInterface
    {
        public uint Id
        {
            get
            {
                return player.Id;
            }
        }
        public string Title
        {
            get
            {
                return player.FullName.Nickname;
            }
        }
        public string Info
        {
            get
            {
                //todo race and class strings
                return player.Phenotype.Race.ToString() + ", " + player.Phenotype.Class.ToString();
            }
        }
        public float Distance
        {
            get
            {
                return player.Transform.Position.HorizontalDistance(hero.Transform.Position) / 100;
            }
        }
        public float DeltaZ
        {
            get
            {
                return (player.Transform.Position.Z - hero.Transform.Position.Z) / 100;
            }
        }

        public PlayerListViewModel(Player player, Hero hero)
        {
            this.player = player;
            this.hero = hero;

            player.Phenotype.PropertyChanged += Phenotype_PropertyChanged;
            player.FullName.PropertyChanged += FullName_PropertyChanged;
            player.Transform.Position.PropertyChanged += PlayerPosition_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
        }

        private void Phenotype_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Race" || e.PropertyName == "Class")
            {
                OnPropertyChanged("Info");
            }
        }

        private void PlayerPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Distance");
            OnPropertyChanged("DeltaZ");
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Distance");
            OnPropertyChanged("DeltaZ");
        }

        private void FullName_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Nickname")
            {
                OnPropertyChanged("Title");
            }
        }

        private readonly Player player;
        private readonly Hero hero;
    }
}
