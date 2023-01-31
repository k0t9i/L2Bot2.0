using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class DropDeletedEvent : EventInterface
    {
        public readonly uint Id;

        public DropDeletedEvent(uint id)
        {
            Id = id;
        }
    }
}
