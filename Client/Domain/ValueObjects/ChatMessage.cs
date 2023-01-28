using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.ValueObjects
{
    public class ChatMessage
    {
        public uint ObjectId { get; set; }
        public ChatChannelEnum Channel { get; set; }
        public string Name { get; set; }
        public string Text { get; set; }

        public ChatMessage(uint objectId, ChatChannelEnum channel, string name, string text)
        {
            ObjectId = objectId;
            Channel = channel;
            Name = name;
            Text = text;
        }
    }
}
