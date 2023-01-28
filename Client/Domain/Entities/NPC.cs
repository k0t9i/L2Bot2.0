using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class NPC : EntityInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public bool IsHostile { get; set; }
        public uint NpcId { get; set; }
        public SpoilStateEnum SpoilState { get; set; }
        public FullName FullName { get; set; }
        public VitalStats VitalStats { get; set; }

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
    }
}
