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
    public class ItemHander : EntityHandler<ItemInterface>
    {
        public override void OnCreate(ItemInterface entity)
        {
            eventBus.Publish(new ItemCreatedEvent(entity));
        }
        public override void OnDelete(ItemInterface entity)
        {
            eventBus.Publish(new ItemDeletedEvent(entity.Id));
        }

        public ItemHander(EntityFactoryInterface<ItemInterface> factory, EventBusInterface eventBus) : base(factory)
        {
            this.eventBus = eventBus;
        }

        private readonly EventBusInterface eventBus;
    }
}
