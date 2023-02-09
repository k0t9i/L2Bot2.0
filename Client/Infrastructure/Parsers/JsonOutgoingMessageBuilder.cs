using Client.Domain;
using Client.Domain.Enums;
using Client.Domain.Parsers;
using Client.Domain.DTO;
using Client.Infrastructure.Parsers.Objects;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Infrastructure.Parsers
{
    public class JsonOutgoingMessageBuilder : OutgoingMessageBuilderInterface
    {
        public string Build<T>(OutgoingMessage<T> message)
        {
            return JsonConvert.SerializeObject(new OutgoingMessage
            {
                Type = GetStringType(message.Type),
                Content = JsonConvert.SerializeObject(message.Content)
            });
        }

        private string GetStringType(OutgoingMessageTypeEnum type)
        {
            return type.ToString();
        }
    }
}
