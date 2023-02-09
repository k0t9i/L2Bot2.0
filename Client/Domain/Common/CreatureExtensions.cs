using Client.Domain.Entities;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Common
{
    public static class CreatureExtensions
    {
        public static float Distance(this CreatureInterface creature, CreatureInterface other)
        {
            return creature.Transform.Position.HorizontalDistance(other.Transform.Position);
        }

        public static float DeltaZ(this CreatureInterface creature, CreatureInterface other)
        {
            return (creature.Transform.Position.Z - other.Transform.Position.Z);
        }
    }
}
