﻿using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Factories;
using Client.Infrastructure.Parsers.Converters;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Infrastructure.Factories
{
    public class ItemFactory : EntityFactoryInterface<ItemInterface>
    {
        public ItemInterface? Create(string data)
        {
            var type = JsonConvert.DeserializeObject<ItemType>(data, settings);

            if (type == null)
            {
                throw new ArgumentNullException(nameof(type));
            }

            switch (type.Type)
            {
                case ItemTypeEnum.Etc:
                    return JsonConvert.DeserializeObject<EtcItem>(data, settings);
                case ItemTypeEnum.Weapon:
                    return JsonConvert.DeserializeObject<WeaponItem>(data, settings);
                default:
                    return JsonConvert.DeserializeObject<EtcItem>(data, settings); //fixme temporary
            }

            throw new ArgumentException("Invalid item type " + type.Type.ToString());
        }

        public void Update(ItemInterface entity, string data)
        {
            JsonConvert.PopulateObject(data, entity, settings);
        }

        private JsonSerializerSettings settings = new JsonSerializerSettings { Converters = { new BooleanConverter() } };

        private class ItemType
        {
            public ItemTypeEnum Type { get; set; }
        }
    }
}
