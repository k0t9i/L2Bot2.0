using Client.Domain.AI;
using Client.Domain.AI.IO;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Infrastructure.AI.IO
{
    public class JsonConfigSerializer : ConfigSerializerInterface
    {
        public string Serialize(Config config)
        {
            return JsonConvert.SerializeObject(config, Formatting.Indented, new JsonSerializerSettings() { Culture = CultureInfo.InvariantCulture, ReferenceLoopHandling = ReferenceLoopHandling.Ignore });
        }
    }
}
