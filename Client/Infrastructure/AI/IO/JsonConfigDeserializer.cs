using Client.Domain.AI;
using Client.Domain.AI.IO;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Infrastructure.AI.IO
{
    public class JsonConfigDeserializer : ConfigDeserializerInterface
    {
        public Config? Deserialize(string data)
        {
            return JsonConvert.DeserializeObject<Config>(data);
        }
    }
}
