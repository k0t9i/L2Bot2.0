using Client.Domain.ValueObjects;

namespace Client.Domain.Entities
{
    public class Hero : EntityInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public FullName FullName { get; set; }
        public VitalStats VitalStats { get; set; }
        public Phenotype Phenotype { get; set; }
        public ExperienceInfo ExperienceInfo { get; set; }
        public PermanentStats PermanentStats { get; set; }
        public VariableStats VariableStats { get; set; }
        public Reputation Reputation { get; set; }
        public InventoryInfo InventoryInfo { get; set; }
        public uint TargetId { get; set; }
        public bool IsStanding { get; set; }

        public Hero(uint id, Transform transform, FullName fullName, VitalStats vitalStats, Phenotype phenotype, ExperienceInfo experienceInfo, PermanentStats permanentStats, VariableStats variableStats, Reputation reputation, InventoryInfo inventoryInfo, uint targetId, bool isStanding)
        {
            Id = id;
            Transform = transform;
            FullName = fullName;
            VitalStats = vitalStats;
            Phenotype = phenotype;
            ExperienceInfo = experienceInfo;
            PermanentStats = permanentStats;
            VariableStats = variableStats;
            Reputation = reputation;
            InventoryInfo = inventoryInfo;
            TargetId = targetId;
            IsStanding = isStanding;
        }
    }
}
