using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class CreatureDeletedEvent : EventInterface
    {
        public readonly uint Id;

        public CreatureDeletedEvent(uint id)
        {
            Id = id;
        }
    }
}
