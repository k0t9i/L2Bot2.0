using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;

namespace Client.Infrastructure.Parsers.Converters
{
    public class RawConverter : JsonConverter
    {
        public override bool CanConvert(Type objectType)
        {
            return true;
        }

        public override object? ReadJson(JsonReader reader, Type objectType, object? existingValue, JsonSerializer serializer)
        {
            if (reader.TokenType == JsonToken.Null)
            {
                return null;
            }

            var raw = JRaw.Create(reader);
            return raw.ToString();
        }

        public override void WriteJson(JsonWriter writer, object? value, JsonSerializer serializer)
        {
            writer.WriteRawValue(value == null ? null : (string)value);
        }
    }
}
