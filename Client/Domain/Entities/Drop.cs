﻿using Client.Domain.Common;
using Client.Domain.ValueObjects;

namespace Client.Domain.Entities
{
    public class Drop : ObservableObject, EntityInterface
    {
        private uint id;
        private Transform transform;
        private uint itemId;
        private uint amount;
        private string name;
        private string iconName;

        public uint Id { get => id; set => id = value; }
        public Transform Transform { get => transform; set { if (value != transform) { transform = value; OnPropertyChanged("Transform"); } } }
        public uint ItemId { get => itemId; set { if (value != itemId) { itemId = value; OnPropertyChanged("ItemId"); } } }
        public uint Amount { get => amount; set { if (value != amount) { amount = value; OnPropertyChanged("Amount"); } } }
        public string Name { get => name; set { if (value != name) { name = value; OnPropertyChanged("Name"); } } }
        public string IconName { get => iconName; set { if (value != iconName) { iconName = value; OnPropertyChanged("IconName"); } } }

        public Drop(uint id, Transform transform, uint itemId, uint amount, string name, string iconName)
        {
            this.id = id;
            this.transform = transform;
            this.itemId = itemId;
            this.amount = amount;
            this.name = name;
            this.iconName = iconName;
        }
    }
}
