using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class NpcCreatedEvent : EventInterface
    {
        public readonly NPC NPC;

        public NpcCreatedEvent(NPC npc)
        {
            NPC = npc;
        }
    }
}
