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
            var baseEntity = entityFactory.Create(content);
            if (baseEntity == null)
            {
                throw new ArgumentNullException(nameof(baseEntity));
            }

            if (operation == MessageOperationEnum.Create)
            {
                var entity = factory.Create(content);

                if (entity == null)
                {
                    throw new ArgumentNullException(nameof(entity));
                }
                entities[baseEntity.Id] = entity;

                OnCreate(entity);
            }
            else if (operation == MessageOperationEnum.Update)
            {
                if (entities.ContainsKey(baseEntity.Id))
                {
                    var entity = entities[baseEntity.Id];
                    factory.Update(entities[entity.Id], content);
                    OnUpdate(entities[entity.Id]);
                }
            }
            else if (operation == MessageOperationEnum.Delete)
            {
                if (entities.ContainsKey(baseEntity.Id))
                {
                    var entity = entities[baseEntity.Id];
                    entities.Remove(entity.Id);
                    OnDelete(entity);
                }
            }
        }

        public T? GetEntity(uint id)
        {
            T? result;
            entities.TryGetValue(id, out result);

            return result;
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

        public EntityHandler(EntityFactoryInterface<T> factory, EntityFactoryInterface<Entity> entityFactory)
        {
            this.factory = factory;
            this.entityFactory = entityFactory;
        }

        private readonly EntityFactoryInterface<T> factory;
        private readonly EntityFactoryInterface<Entity> entityFactory;
        private Dictionary<uint, T> entities = new Dictionary<uint, T>();
    }
}
