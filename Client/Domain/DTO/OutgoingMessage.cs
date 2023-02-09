using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.DTO
{
    public class OutgoingMessage<T>
    {
        public readonly OutgoingMessageTypeEnum Type;
        public readonly T? Content;

        public OutgoingMessage(OutgoingMessageTypeEnum type, T? content = default)
        {
            Type = type;
            Content = content;
        }
    }

    public class EmptyOutgoingMessage : OutgoingMessage<uint>
    {
        public EmptyOutgoingMessage(OutgoingMessageTypeEnum type) : base(type)
        {
        }
    }
}
