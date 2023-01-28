using Newtonsoft.Json;
using Client.Domain.Enums;

namespace Client.Infrastructure.Parsers
{
    public class JsonMessageParser : Domain.Parsers.MessageParserInterface
    {
        public Domain.DTO.Message Parse(string message)
        {
            try
            {
                var obj = JsonConvert.DeserializeObject<Objects.Message>(message);
                if (obj == null)
                {
                    throw new Domain.Exception.ParserException();
                }
                return new Domain.DTO.Message(GetType(obj.Type), GetOperation(obj.Operation), obj.Content ?? "");
            }
            catch(JsonException)
            {
                throw new Domain.Exception.ParserException();
            }
        }

        private MessageTypeEnum GetType(string? type)
        {
            switch (type)
            {
                case "hero":
                    return MessageTypeEnum.Hero;
                case "drop":
                    return MessageTypeEnum.Drop;
                case "npc":
                    return MessageTypeEnum.NPC;
                case "player":
                    return MessageTypeEnum.Player;
                case "skill":
                    return MessageTypeEnum.Skill;
                case "item":
                    return MessageTypeEnum.Item;
                case "abnormalEffect":
                    return MessageTypeEnum.AbnormalEffect;
                case "chat":
                    return MessageTypeEnum.Chat;
            }

            return MessageTypeEnum.None;
        }

        private MessageOperationEnum GetOperation(string? type)
        {
            switch (type)
            {
                case "create":
                    return MessageOperationEnum.Create;
                case "update":
                    return MessageOperationEnum.Update;
                case "delete":
                    return MessageOperationEnum.Delete;
            }

            return MessageOperationEnum.None;
        }
    }
}
