using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class ItemDeletedEvent : EventInterface
    {
        public readonly uint Id;

        public ItemDeletedEvent(uint id)
        {
            Id = id;
        }
    }
}
