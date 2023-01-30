using Client.Domain.Common;
using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class NpcListViewModel : NotifyPropertyChanged, CreatureListViewModelInterface
    {
        public uint Id
        {
            get
            {
                return npc.Id;
            }
        }
        public string Title
        {
            get
            {
                string result = "";

                if (npc.IsDead())
                {
                    result += "Dead ";
                }
                result += npc.FullName.Nickname + "[" + npc.NpcId + "]";

                return result;
            }
        }
        public string Info
        {
            get
            {
                string result = "Npc";

                if (npc.IsHostile)
                {
                    result = "Monster";
                    if (npc.AggroRadius > 0)
                    {
                        result += "*";
                    }
                }
                result += " " + npc.Level + "lvl";
                return result;
            }
        }
        public float Distance
        {
            get
            {
                return npc.Transform.Position.HorizontalDistance(hero.Transform.Position) / 100;
            }
        }
        public float DeltaZ
        {
            get
            {
                return (npc.Transform.Position.Z - hero.Transform.Position.Z) / 100;
            }
        }

        public NpcListViewModel(NPC npc, Hero hero)
        {
            this.npc = npc;
            this.hero = hero;

            npc.VitalStats.PropertyChanged += VitalStats_PropertyChanged;
            npc.FullName.PropertyChanged += FullName_PropertyChanged;
            npc.Transform.Position.PropertyChanged += NpcPosition_PropertyChanged;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
        }

        private void NpcPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
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

        private void VitalStats_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Hp" || e.PropertyName == "MaxHp")
            {
                OnPropertyChanged("Title");
            }
        }

        private readonly NPC npc;
        private readonly Hero hero;
    }
}
