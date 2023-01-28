using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Factories;
using Client.Domain.Service;

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
                    result = (HandlerInterface?)serviceProvider.GetService(typeof(EntityHandler<Hero>));
                    break;
                case MessageTypeEnum.Drop:
                    result = (HandlerInterface?)serviceProvider.GetService(typeof(EntityHandler<Drop>));
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
