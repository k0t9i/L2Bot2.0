using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class WeaponItem : BaseItem, ItemInterface
    {
        public WeaponTypeEnum WeaponType { get; set; }
        public CrystalTypeEnum CrystalType { get; set; }
        public byte SoulshotCount { get; set; }
        public byte SpiritshotCount { get; set; }
        public bool IsEquipped { get; set; }

        public WeaponItem(uint id, uint itemId, ItemTypeEnum type, string name, string iconName, string description, int mana, uint weight, WeaponTypeEnum weaponType, CrystalTypeEnum crystalType, byte soulshotCount, byte spiritshotCount, bool isEquipped) :
            base(id, itemId, type, name, iconName, description, mana, weight)
        {
            WeaponType = weaponType;
            CrystalType = crystalType;
            SoulshotCount = soulshotCount;
            SpiritshotCount = spiritshotCount;
            IsEquipped = isEquipped;
        }
    }
}
