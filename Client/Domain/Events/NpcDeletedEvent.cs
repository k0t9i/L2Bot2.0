using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class NpcDeletedEvent : EventInterface
    {
        public readonly uint Id;

        public NpcDeletedEvent(uint id)
        {
            Id = id;
        }
    }
}
