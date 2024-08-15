using Client.Domain.Common;
using Client.Domain.ValueObjects;

namespace Client.Domain.AI.Combat
{
    public class CombatZone : ObservableObject
    {
        public CombatZone(Vector3 center, float radius)
        {
            Center = center;
            Radius = radius;
        }

        public bool IsInside(Vector3 point)
        {
            return Center.HorizontalDistance(point) <= Radius;
        }

        public Vector3 Center { get { return center; } set { if (center != value) { center = value; OnPropertyChanged(); } }}
        public float Radius { get { return radius; } set { if (radius != value) { radius = value; OnPropertyChanged(); } }}

        private float radius;
        private Vector3 center = new Vector3(0, 0, 0);
    }
}
