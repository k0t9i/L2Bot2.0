using Client.Domain.Enums;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public class Player : EntityInterface
    {
        public uint Id { get; set; }
        public Transform Transform { get; set; }
        public FullName FullName { get; set; }
        public Phenotype Phenotype { get; set; }

        public Player(uint id, Transform transform, FullName fullName, Phenotype phenotype)
        {
            Id = id;
            Transform = transform;
            FullName = fullName;
            Phenotype = phenotype;
        }
    }
}
