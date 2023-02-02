using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class EtcItem : BaseItem
    {
        public uint Amount { get => amount; set => amount = value; }
        public bool IsQuest { get; set; }
        public bool IsAutoused { get => isAutoused; set => isAutoused = value; }
        public EtcItem(uint id, uint itemId, ItemTypeEnum type, string name, string iconName, string description, int mana, uint weight, uint amount, bool isQuest, bool isAutoused) :
            base(id, itemId, type, name, iconName, description, mana, weight)
        {
            this.amount = amount;
            IsQuest = isQuest;
            this.isAutoused = isAutoused;
        }

        private uint amount;
        private bool isAutoused;
    }
}
