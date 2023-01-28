using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Entities;

namespace Client.Domain.Factories
{ 
    public interface EntityFactoryInterface<T>
    {
        public T? Create(string data);
        public void Update(T entity, string data);
    }
}
