using Client.Domain.Enums;

namespace Client.Domain.DTO
{
    public class Message
    {
        public readonly MessageTypeEnum Type;
        public readonly MessageOperationEnum Operation;
        public readonly string Content;

        public Message(MessageTypeEnum type, MessageOperationEnum operation, string content)
        {
            Type = type;
            Operation = operation;
            Content = content;
        }
    }
}
