using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.DTO;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Events;
using Client.Domain.Factories;
using Client.Domain.ValueObjects;

namespace Client.Domain.Service
{
    public class ChatMessageHandler : HandlerInterface
    {
        public void Update(MessageOperationEnum operation, string content)
        {
            var message = factory.Create(content);
            if (operation == MessageOperationEnum.Create)
            {
                if (message == null)
                {
                    throw new ArgumentNullException(nameof(message));
                }
                eventBus.Publish(new ChatMessageCreatedEvent(message));
            }
        }

        public ChatMessageHandler(EntityFactoryInterface<ChatMessage> factory, EventBusInterface eventBus)
        {
            this.factory = factory;
            this.eventBus = eventBus;
        }

        private readonly EntityFactoryInterface<ChatMessage> factory;
        private readonly EventBusInterface eventBus;
    }
}
