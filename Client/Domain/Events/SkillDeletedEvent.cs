using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class SkillDeletedEvent : EventInterface
    {
        public readonly uint Id;

        public SkillDeletedEvent(uint id)
        {
            Id = id;
        }
    }
}
