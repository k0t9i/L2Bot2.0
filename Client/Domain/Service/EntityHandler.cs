using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.DTO;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Factories;

namespace Client.Domain.Service
{
    public class EntityHandler<T> : HandlerInterface where T : class, EntityInterface
    {
        public void Update(MessageOperationEnum operation, string content)
        {
            var entity = factory.Create(content);
            if (operation == MessageOperationEnum.Create)
            {
                if (entity == null)
                {
                    throw new ArgumentNullException(nameof(entity));
                }
                entities[entity.Id] = entity;

                OnCreate(entity);
            }
            else if (operation == MessageOperationEnum.Update)
            {
                if (entity != null && entities.ContainsKey(entity.Id))
                {
                    factory.Update(entities[entity.Id], content);
                    OnUpdate(entity);
                }
            }
            else if (operation == MessageOperationEnum.Delete)
            {
                if (entity != null)
                {
                    entities.Remove(entity.Id);
                    OnDelete(entity);
                }
            }
        }

        public virtual void OnCreate(T entity)
        {

        }

        public virtual void OnUpdate(T entity)
        {

        }

        public virtual void OnDelete(T entity)
        {

        }

        public EntityHandler(EntityFactoryInterface<T> factory)
        {
            this.factory = factory;
        }

        private readonly EntityFactoryInterface<T> factory;
        private Dictionary<uint, T> entities = new Dictionary<uint, T>();
    }
}
