using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Enums;
using Client.Domain.Service;

namespace Client.Domain.Factories
{
    public interface EntityHandlerFactoryInterface
    {
        HandlerInterface GetHandler(MessageTypeEnum type);
    }
}
