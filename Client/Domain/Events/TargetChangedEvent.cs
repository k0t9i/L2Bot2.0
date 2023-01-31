using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class TargetChangedEvent : EventInterface
    {
        public readonly Hero Hero;

        public TargetChangedEvent(Hero hero)
        {
            Hero = hero;
        }
    }
}
