using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Entities;
using Client.Domain.Factories;
using Client.Infrastructure.Parsers.Converters;

namespace Client.Infrastructure.Factories
{
    public class EntityFactory<T> : EntityFactoryInterface<T> where T : class
    {
        public T? Create(string data)
        {
            return JsonConvert.DeserializeObject<T>(data, settings);
        }

        public void Update(T entity, string data)
        {
            JsonConvert.PopulateObject(data, entity, settings);
        }

        private JsonSerializerSettings settings = new JsonSerializerSettings { Converters = { new BooleanConverter() } };
    }
}
