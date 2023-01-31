using Client.Domain.Entities;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public class ChatMessageCreatedEvent : EventInterface
    {
        public readonly ChatMessage Message;

        public ChatMessageCreatedEvent(ChatMessage message)
        {
            Message = message;
        }
    }
}
