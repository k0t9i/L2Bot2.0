using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class SkillCreatedEvent : EventInterface
    {
        public readonly Skill Skill;

        public SkillCreatedEvent(Skill skill)
        {
            Skill = skill;
        }
    }
}

