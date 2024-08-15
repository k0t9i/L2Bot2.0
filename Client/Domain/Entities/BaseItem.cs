using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public abstract class BaseItem
    {
        public uint Id { get; set; }
        public uint ItemId { get; set; }
        public ItemTypeEnum Type { get; set; }
        public string Name { get; set; }
        public string IconName { get; set; }
        public string Description { get; set; }
        public int Mana { get; set; }
        public uint Weight { get; set; }
        public virtual string FullDescription => Description;

        public BaseItem(uint id, uint itemId, ItemTypeEnum type, string name, string iconName, string description, int mana, uint weight)
        {
            Id = id;
            ItemId = itemId;
            Type = type;
            Name = name;
            IconName = iconName;
            Description = description;
            Mana = mana;
            Weight = weight;
        }
    }
}
