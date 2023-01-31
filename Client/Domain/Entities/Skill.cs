using Client.Domain.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class Skill : ObservableObject, EntityInterface
    {
        public uint Id { get; set; }
        public uint Level { get => level; set { if (value != level) { level = value; OnPropertyChanged(); } } }
        public bool IsActive { get; set; }
        public uint Cost { get => cost; set { if (value != cost) { cost = value; OnPropertyChanged(); } } }
        public int Range { get => range; set { if (value != range) { range = value; OnPropertyChanged(); } } }
        public string Name { get => name; set { if (value != name) { name = value; OnPropertyChanged(); } } }
        public string Description { get => description; set { if (value != description) { description = value; OnPropertyChanged(); } } }
        public string IconName { get => iconName; set { if (value != iconName) { iconName = value; OnPropertyChanged(); } } }
        public bool IsToggled { get => isToggled; set { if (value != isToggled) { isToggled = value; OnPropertyChanged(); } } }
        public bool IsCasting { get => isCasting; set { if (value != isCasting) { isCasting = value; OnPropertyChanged(); } } }
        public bool IsReloading { get => isReloading; set { if (value != isReloading) { isReloading = value; OnPropertyChanged(); } } }
        public bool IsReadyToUse { get => isReadyToUse; set { if (value != isReadyToUse) { isReadyToUse = value; OnPropertyChanged(); } } }

        public Skill(uint id, uint level, bool isActive, uint cost, int range, string name, string description, string iconName, bool isToggled, bool isCasting, bool isReloading, bool isReadyToUse)
        {
            Id = id;
            this.level = level;
            IsActive = isActive;
            this.cost = cost;
            this.range = range;
            Name = name;
            this.description = description;
            IconName = iconName;
            this.isToggled = isToggled;
            this.isCasting = isCasting;
            this.isReloading = isReloading;
            this.isReadyToUse = isReadyToUse;
        }

        private string description;
        private uint cost;
        private int range;
        private bool isToggled;
        private bool isCasting;
        private bool isReloading;
        private bool isReadyToUse;
        private uint level;
        private string name;
        private string iconName;
    }
}
