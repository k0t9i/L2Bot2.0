namespace Client.Domain.ValueObjects
{
    public class InventoryInfo
    {
        public uint MaxWeight { get; set; }
        public uint Weight { get; set; }
        public uint Slots { get; set; }

        public InventoryInfo(uint maxWeight, uint weight, uint slots)
        {
            MaxWeight = maxWeight;
            Weight = weight;
            Slots = slots;
        }
    }
}
