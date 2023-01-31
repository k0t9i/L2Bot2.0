using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Events
{
    public interface EventBusInterface
    {
        void Subscrbe<T>(EventHandlerInterface<T> handler) where T : EventInterface;
        void Unsubscrbe<T>(EventHandlerInterface<T> handler) where T : EventInterface;
        void Publish<T>(T @event) where T : EventInterface;
    }
}
