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
    public class DropHandler : EntityHandler<Drop>
    {
        public override void OnCreate(Drop entity)
        {
            eventBus.Publish(new DropCreatedEvent(entity));
        }
        public override void OnDelete(Drop entity)
        {
            eventBus.Publish(new DropDeletedEvent(entity.Id));
        }

        public DropHandler(EntityFactoryInterface<Drop> factory, EntityFactoryInterface<Entity> entityFactory, EventBusInterface eventBus) : base(factory, entityFactory)
        {
            this.eventBus = eventBus;
        }

        private readonly EventBusInterface eventBus;
    }
}
