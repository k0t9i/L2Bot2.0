using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Factories;
using Client.Domain.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Service
{
    public class PlayerHandler : EntityHandler<Player>
    {
        public override void OnCreate(Player entity)
        {
            eventBus.Publish(new CreatureCreatedEvent(entity));
        }
        public override void OnDelete(Player entity)
        {
            eventBus.Publish(new CreatureDeletedEvent(entity.Id));
        }

        public PlayerHandler(EntityFactoryInterface<Player> factory, EventBusInterface eventBus) : base(factory)
        {
            this.eventBus = eventBus;
        }

        private readonly EventBusInterface eventBus;
    }
}
