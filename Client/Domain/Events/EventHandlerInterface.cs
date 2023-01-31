using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public interface EventHandlerInterface
    {
    }

    public interface EventHandlerInterface<T> : EventHandlerInterface where T : EventInterface
    {
        void Handle(T @event);
    }
}
