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
    public class PlayerHandler : EntityHandler<Player>, EventHandlerInterface<TargetChangedEvent>
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

        public void Handle(TargetChangedEvent @event)
        {
            var target = GetEntity(@event.Hero.TargetId);
            if (target == null)
            {
                return;
            }
            @event.Hero.Target = target;
        }

        private readonly EventBusInterface eventBus;
    }
}
