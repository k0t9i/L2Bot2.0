using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Factories;
using Client.Domain.Service;
using Microsoft.Extensions.DependencyInjection;

namespace Client.Infrastructure.Factories
{
    public class EntityHandlerFactory : EntityHandlerFactoryInterface
    {
        private readonly IServiceProvider serviceProvider;

        public HandlerInterface GetHandler(MessageTypeEnum type)
        {
            HandlerInterface? result = null;

            switch (type)
            {
                case MessageTypeEnum.Hero:
                    result = serviceProvider.GetService<HeroHandler>();
                    break;
                case MessageTypeEnum.Drop:
                    result = serviceProvider.GetService<DropHandler>();
                    break;
                case MessageTypeEnum.NPC:
                    result = serviceProvider.GetService<NpcHandler>();
                    break;
                case MessageTypeEnum.Player:
                    result = serviceProvider.GetService<PlayerHandler>();
                    break;
                case MessageTypeEnum.Chat:
                    result = serviceProvider.GetService<ChatMessageHandler>();
                    break;
                case MessageTypeEnum.Skill:
                    result = serviceProvider.GetService<SkillHandler>();
                    break;
                case MessageTypeEnum.Item:
                    result = serviceProvider.GetService<ItemHander>();
                    break;
            }
            
            if (result == null)
            {
                throw new ArgumentException("Handler not found " + type.ToString());
            }

            return result;
        }

        public EntityHandlerFactory(IServiceProvider serviceProvider)
        {
            this.serviceProvider = serviceProvider;
        }
    }
}
