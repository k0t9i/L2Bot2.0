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
    public class NPC : ObservableObject, EntityInterface, CreatureInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public bool IsHostile { get; set; }
        public uint NpcId { get; set; }
        public SpoilStateEnum SpoilState { get { return spoilState; } set { if (spoilState != value) { spoilState = value; OnPropertyChanged(); } } }
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
                string result = FullName.Nickname;

                if (VitalStats.IsDead)
                {
                    result += " (dead)";
                }

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
                result += "<" + NpcId + ">";
                result += " " + Level + "lvl";
                return result;
            }
        }
        public CreatureTypeEnum Type { get => CreatureTypeEnum.NPC; }
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
            if (e.PropertyName == "IsDead")
            {
                OnPropertyChanged("Name");
            }
        }

        private uint level;
        private uint aggroRadius;
        private VitalStats vitalStats;
        private FullName fullName;
        private SpoilStateEnum spoilState;
    }
}
