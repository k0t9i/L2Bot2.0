using Client.Domain.ValueObjects;

namespace Client.Domain.Entities
{
    public class Drop : EntityInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public uint ItemId { get; set; }
        public uint Amount { get; set; }
        public string Name { get; set; }
        public string IconName { get; set; }

        public Drop(uint id, Transform transform, uint itemId, uint amount, string name, string iconName)
        {
            Id = id;
            Transform = transform;
            ItemId = itemId;
            Amount = amount;
            Name = name;
            IconName = iconName;
        }
    }
}
