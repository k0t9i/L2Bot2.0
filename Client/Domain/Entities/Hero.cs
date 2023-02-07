using Client.Domain.Common;
using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System.ComponentModel;
using System.Diagnostics;

namespace Client.Domain.Entities
{
    public class Hero : ObservableObject, EntityInterface, CreatureInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public VitalStats VitalStats { get; set; }
        public ExperienceInfo ExperienceInfo { get; set; }
        public PermanentStats PermanentStats { get; set; }
        public VariableStats VariableStats { get; set; }
        public Reputation Reputation { get; set; }
        public InventoryInfo InventoryInfo { get; set; }
        public uint TargetId { get => targetId; set { if (value != targetId) { targetId = value; OnPropertyChanged("TargetId"); } } }
        public CreatureInterface? Target { get => target; set { if (value != target) { target = value; OnPropertyChanged("Target"); } } }
        public bool IsStanding { get; set; }
        public CreatureTypeEnum Type { get => CreatureTypeEnum.Hero; }
        public FullName FullName
        {
            get => fullName;
            set
            {
                fullName = value;
                if (fullName != null)
                {
                    FullName.PropertyChanged += FullName_PropertyChanged; ;
                }
            }
        }

        public Phenotype Phenotype
        {
            get => phenotype;
            set
            {
                phenotype = value;
                if (phenotype != null)
                {
                    Phenotype.PropertyChanged += Phenotype_PropertyChanged;
                }
            }
        }

        public string Name
        {
            get
            {
                return FullName.Nickname;
            }
        }
        public string BriefInfo
        {
            get
            {
                //todo race and class strings
                return Phenotype.Race.ToString() + ", " + Phenotype.Class.ToString();
            }
        }
        public uint AggroRadius { get; set; } = 0;

        public Hero(uint id, Transform transform, FullName fullName, VitalStats vitalStats, Phenotype phenotype, ExperienceInfo experienceInfo, PermanentStats permanentStats, VariableStats variableStats, Reputation reputation, InventoryInfo inventoryInfo, uint targetId, bool isStanding)
        {
            Id = id;
            Transform = transform;
            this.fullName = FullName = fullName;
            VitalStats = vitalStats;
            this.phenotype = Phenotype = phenotype;
            ExperienceInfo = experienceInfo;
            PermanentStats = permanentStats;
            VariableStats = variableStats;
            Reputation = reputation;
            InventoryInfo = inventoryInfo;
            TargetId = targetId;
            IsStanding = isStanding;
        }

        private void Phenotype_PropertyChanged(object? sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Race" || e.PropertyName == "Class")
            {
                OnPropertyChanged("BriefInfo");
            }
        }

        private void FullName_PropertyChanged(object? sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Nickname")
            {
                OnPropertyChanged("Name");
            }
        }

        private FullName fullName;
        private Phenotype phenotype;
        private CreatureInterface? target;
        private uint targetId;
    }
}
