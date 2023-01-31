using Client.Domain.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Client.Infrastructure.Events
{
    public class InMemoryEventBus : EventBusInterface
    {
        public void Publish<T>(T @event) where T : EventInterface
        {
            var type = @event.GetType();
            if (subscribers.ContainsKey(type))
            {
                foreach (var subscriber in subscribers[type])   
                {
                    (subscriber as EventHandlerInterface<T>)?.Handle(@event);
                }
            }
        }

        public void Subscrbe<T>(EventHandlerInterface<T> handler) where T : EventInterface
        {
            var type = typeof(T);

            if (!subscribers.ContainsKey(type))
            {
                subscribers[type] = new List<EventHandlerInterface>();
            }

            subscribers[type].Add(handler);
        }

        public void Unsubscrbe<T>(EventHandlerInterface<T> handler) where T : EventInterface
        {
            var type = typeof(T);

            if (subscribers.ContainsKey(type))
            {
                subscribers[type].Remove(handler);
            }
        }

        private Dictionary<Type, List<EventHandlerInterface>> subscribers = new Dictionary<Type, List<EventHandlerInterface>>();
    }
}
