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
    public class NPC : NotifyPropertyChanged, EntityInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public bool IsHostile { get; set; }
        public uint NpcId { get; set; }
        public SpoilStateEnum SpoilState { get; set; }
        public FullName FullName { get; set; }
        public VitalStats VitalStats { get; set; }
        public uint Level { get => level; set { if (value != level) { level = value; OnPropertyChanged("Level"); } } }
        public uint AggroRadius { get => aggroRadius; set { if (value != aggroRadius) { aggroRadius = value; OnPropertyChanged("AggroRadius"); } } }

        public NPC(uint id, Transform transform, bool isHostile, uint npcId, SpoilStateEnum spoilState, FullName fullName, VitalStats vitalStats)
        {
            Id = id;
            Transform = transform;
            IsHostile = isHostile;
            NpcId = npcId;
            SpoilState = spoilState;
            FullName = fullName;
            VitalStats = vitalStats;
        }

        public bool IsDead()
        {
            return VitalStats.MaxHp > 0 && VitalStats.Hp <= 0;
        }

        private uint level;
        private uint aggroRadius;
    }
}
