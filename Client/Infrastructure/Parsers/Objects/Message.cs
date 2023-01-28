using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Infrastructure.Parsers.Converters;

namespace Client.Infrastructure.Parsers.Objects
{
    internal class Message
    {
        [JsonProperty]
        public string? Type { get; private set; }
        [JsonProperty]
        public string? Operation { get; private set; }
        [JsonProperty]
        [JsonConverter(typeof(RawConverter))]
        public string? Content { get; private set; }
    }
}
