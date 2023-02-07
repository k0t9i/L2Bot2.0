using Client.Domain.Common;
using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class Player : ObservableObject, EntityInterface, CreatureInterface
    {
        private FullName fullName;
        private Phenotype phenotype;
        private VitalStats vitalStats = new VitalStats(0, 0, 0, 0, 0, 0);

        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public FullName FullName
        {
            get => fullName;
            set
            {
                fullName = value;
                if (fullName != null)
                {
                    FullName.PropertyChanged += FullName_PropertyChanged;
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

        public VitalStats VitalStats { get => vitalStats; set => vitalStats = value; }
        public CreatureTypeEnum Type { get => CreatureTypeEnum.Player; }
        public uint AggroRadius { get; set; } = 0;

        public Player(uint id, Transform transform, FullName fullName, Phenotype phenotype)
        {
            Id = id;
            Transform = transform;
            this.fullName = FullName = fullName;
            this.phenotype = Phenotype = phenotype;
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
    }
}
