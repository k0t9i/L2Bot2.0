using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class DropCreatedEvent : EventInterface
    {
        public readonly Drop Drop;

        public DropCreatedEvent(Drop drop)
        {
            Drop = drop;
        }
    }
}
