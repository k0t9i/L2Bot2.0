using Client.Domain.Common;
using System;

namespace Client.Domain.ValueObjects
{
    public class Vector3 : ObservableObject, ICloneable
    {
        private float x;
        private float y;
        private float z;

        public float X { get => x; set { if (value != x) { x = value; OnPropertyChanged("X"); } } }
        public float Y { get => y; set { if (value != y) { y = value; OnPropertyChanged("Y"); } } }
        public float Z { get => z; set { if (value != z) { z = value; OnPropertyChanged("Z"); } } }

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

        public override bool Equals(object? other)
        {
            if (!(other is Vector3))
            {
                return false;
            }

            var obj = (Vector3)other;
            return MathF.Abs(x - obj.x) < float.Epsilon && MathF.Abs(y - obj.y) < float.Epsilon && MathF.Abs(z - obj.z) < float.Epsilon;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public bool ApproximatelyEquals(Vector3 other, float epsilon, bool withZ = false)
        {
            var equals = MathF.Abs(x - other.x) < epsilon && MathF.Abs(y - other.y) < epsilon;
            if (withZ)
            {
                equals = equals && MathF.Abs(z - other.z) < epsilon;
            }

            return equals;
        }

        public float GetAngleBetweenDegree(Vector3 other)
        {
            return GetAngleBetween(other) / MathF.PI * 180;
        }

        public float GetAngleBetween(Vector3 other)
        {
            return MathF.Acos(DotProduct(other) / (Distance(Zero) * other.Distance(Zero)));
        }

        public float DotProduct(Vector3 other)
        {
            return x * other.x + y * other.y + z * other.z;
        }

        public float Distance(Vector3 other)
        {
            return MathF.Sqrt(MathF.Pow(x - other.x, 2) + MathF.Pow(y - other.y, 2) + MathF.Pow(z - other.z, 2));
        }

        public object Clone()
        {
            return MemberwiseClone();
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
        }

        public static readonly Vector3 Zero = new Vector3(0, 0, 0);
    }
}
