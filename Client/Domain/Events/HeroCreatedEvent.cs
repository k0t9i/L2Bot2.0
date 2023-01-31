using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class HeroCreatedEvent : EventInterface
    {
        public readonly Hero Hero;

        public HeroCreatedEvent(Hero hero)
        {
            Hero = hero;
        }
    }
}
