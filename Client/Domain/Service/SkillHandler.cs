using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Factories;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Service
{
    public class SkillHandler : EntityHandler<Skill>
    {
        public override void OnCreate(Skill entity)
        {
            eventBus.Publish(new SkillCreatedEvent(entity));
        }
        public override void OnDelete(Skill entity)
        {
            eventBus.Publish(new SkillDeletedEvent(entity.Id));
        }

        public SkillHandler(EntityFactoryInterface<Skill> factory, EventBusInterface eventBus) : base(factory)
        {
            this.eventBus = eventBus;
        }

        private readonly EventBusInterface eventBus;
    }
}
