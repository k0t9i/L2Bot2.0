using Client.Domain.Common;
using System;

namespace Client.Domain.ValueObjects
{
    public class Vector3 : NotifyPropertyChanged
    {
        private float x;
        private float y;
        private float z;

        public float X { get => x; set { if (value != x) { x = value; OnPropertyChanged("X"); } } }
        public float Y { get => y; set { if (value != y) { y = value; OnPropertyChanged("Y"); } } }
        public float Z { get => z; set { if (value != z) { z = value; OnPropertyChanged("X"); } } }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public float HorizontalSqrDistance(Vector3 other)
        {
            return MathF.Pow(x - other.x, 2) + MathF.Pow(y - other.y, 2);
        }

        public float HorizontalDistance(Vector3 other)
        {
            return MathF.Sqrt(HorizontalSqrDistance(other));
        }
    }
}
