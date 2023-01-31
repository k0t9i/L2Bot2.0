using Client.Domain.Common;
using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class NPC : NotifyPropertyChanged, EntityInterface, CreatureInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public bool IsHostile { get; set; }
        public uint NpcId { get; set; }
        public SpoilStateEnum SpoilState { get; set; }
        public FullName FullName
        {
            get => fullName;
            set
            {
                fullName = value;
                if (fullName != null)
                {
                    FullName.PropertyChanged += FullName_PropertyChanged;
                }
            }
        }
        public VitalStats VitalStats
        {
            get => vitalStats;
            set
            {
                vitalStats = value;
                if (vitalStats != null)
                {
                    VitalStats.PropertyChanged += VitalStats_PropertyChanged;
                }
            }
        }
        public uint Level
        {
            get => level;
            set
            {
                if (value != level)
                {
                    level = value;
                    OnPropertyChanged("Level");
                    OnPropertyChanged("BriefInfo");
                }
            }
        }
        public uint AggroRadius
        {
            get => aggroRadius;
            set
            {
                if (value != aggroRadius)
                {
                    aggroRadius = value;
                    OnPropertyChanged("AggroRadius");
                    OnPropertyChanged("BriefInfo");
                }
            }
        }
        public string Name
        {
            get
            {
                string result = "";

                if (IsDead())
                {
                    result += "Dead ";
                }
                result += FullName.Nickname + "[" + NpcId + "]";

                return result;
            }
        }

        public string BriefInfo
        {
            get
            {
                string result = "Npc";

                if (IsHostile)
                {
                    result = "Monster";
                    if (AggroRadius > 0)
                    {
                        result += "*";
                    }
                }
                result += " " + Level + "lvl";
                return result;
            }
        }

        public NPC(uint id, Transform transform, bool isHostile, uint npcId, SpoilStateEnum spoilState, FullName fullName, VitalStats vitalStats)
        {
            Id = id;
            Transform = transform;
            IsHostile = isHostile;
            NpcId = npcId;
            SpoilState = spoilState;
            this.fullName = FullName = fullName;
            this.vitalStats = VitalStats = vitalStats;
        }

        private void FullName_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Nickname")
            {
                OnPropertyChanged("Name");
            }
        }

        private void VitalStats_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Hp" || e.PropertyName == "MaxHp")
            {
                OnPropertyChanged("Name");
            }
        }

        public bool IsDead()
        {
            return VitalStats.MaxHp > 0 && VitalStats.Hp <= 0;
        }

        private uint level;
        private uint aggroRadius;
        private VitalStats vitalStats;
        private FullName fullName;
    }
}
