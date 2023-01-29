using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.DTO;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Factories;
using Client.Domain.ValueObjects;
using Client.Domain.ViewModels;

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
                messages.Add(message);
                mainViewModel.AddChatMessage(message);
            }
        }

        public ChatMessageHandler(EntityFactoryInterface<ChatMessage> factory, MainViewModelInterface mainViewModel)
        {
            this.factory = factory;
            this.mainViewModel = mainViewModel;
        }

        private readonly EntityFactoryInterface<ChatMessage> factory;
        private readonly MainViewModelInterface mainViewModel;
        private List<ChatMessage> messages = new List<ChatMessage>();
    }
}
