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
    public class ItemHander : EntityHandler<BaseItem>
    {
        public override void OnCreate(BaseItem entity)
        {
            eventBus.Publish(new ItemCreatedEvent(entity));
        }
        public override void OnDelete(BaseItem entity)
        {
            eventBus.Publish(new ItemDeletedEvent(entity.Id));
        }

        public ItemHander(EntityFactoryInterface<BaseItem> factory, EventBusInterface eventBus) : base(factory)
        {
            this.eventBus = eventBus;
        }

        private readonly EventBusInterface eventBus;
    }
}
