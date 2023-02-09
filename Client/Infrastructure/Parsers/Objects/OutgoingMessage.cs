using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Infrastructure.Parsers.Converters;

namespace Client.Infrastructure.Parsers.Objects
{
    internal class OutgoingMessage
    {
        public string Type { get; set; } = "";
        [JsonConverter(typeof(RawConverter))]
        public string Content { get; set; } = "";
    }
}
