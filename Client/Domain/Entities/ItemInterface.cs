using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public interface ItemInterface : EntityInterface
    {
        uint ItemId { get; set; }
        ItemTypeEnum Type { get; set; }
        string Name { get; set; }
        string IconName { get; set; }
        string Description { get; set; }
        int Mana { get; set; }
        uint Weight { get; set; }
        uint Amount { get; set; }
        bool IsQuest { get; set; }
        bool IsAutoused { get; set; }
        string FullDescription { get; }
    }
}
