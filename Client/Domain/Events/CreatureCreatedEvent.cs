using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class CreatureCreatedEvent : EventInterface
    {
        public readonly CreatureInterface Creature;

        public CreatureCreatedEvent(CreatureInterface creature)
        {
            Creature = creature;
        }
    }
}
