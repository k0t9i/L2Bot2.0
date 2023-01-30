using Client.Domain.Common;

namespace Client.Domain.ValueObjects
{
    public class InventoryInfo : NotifyPropertyChanged
    {
        private uint maxWeight;
        private uint weight;
        private uint slots;

        public uint MaxWeight { get => maxWeight; set { if (value != maxWeight) { maxWeight = value; OnPropertyChanged("MaxWeight"); } } }
        public uint Weight { get => weight; set { if (value != weight) { weight = value; OnPropertyChanged("Weight"); } } }
        public uint Slots { get => slots; set { if (value != slots) { slots = value; OnPropertyChanged("Slots"); } } }

        public InventoryInfo(uint maxWeight, uint weight, uint slots)
        {
            this.maxWeight = maxWeight;
            this.weight = weight;
            this.slots = slots;
        }
    }
}
