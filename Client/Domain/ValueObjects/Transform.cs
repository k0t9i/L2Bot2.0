using Client.Domain.Common;
using System;

namespace Client.Domain.ValueObjects
{
    public class Transform : ObservableObject
    {
        private Vector3 position;
        private Vector3 rotation;
        private Vector3 velocity;
        private Vector3 acceleration;

        public Vector3 Position { get => position; set { if (value != position) { position = value; OnPropertyChanged(); } } }
        public Vector3 Rotation {
            get => rotation;
            set
            {
                if (value != rotation)
                {
                    rotation = value;
                    OnPropertyChanged();
                    OnPropertyChanged("Direction");
                }
            } 
        }

        private void Rotation_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Y")
            {
                OnPropertyChanged("Direction");
            }
        }

        public Vector3 Velocity { get => velocity; set { if (value != velocity) { velocity = value; OnPropertyChanged(); } } }
        public Vector3 Acceleration { get => acceleration; set { if (value != acceleration) { acceleration = value; OnPropertyChanged(); } } }

        public Vector3 Direction
        {
            get
            {
                float deg = Rotation.Y / 65535 * 2 * MathF.PI;

                return new Vector3(MathF.Cos(deg), MathF.Sin(deg), 0);
            }
        }

        public Transform(Vector3 position, Vector3 rotation, Vector3 velocity, Vector3 acceleration)
        {
            this.position = position;
            this.rotation = rotation;
            this.velocity = velocity;
            this.acceleration = acceleration;
            rotation.PropertyChanged += Rotation_PropertyChanged;
        }
    }
}
