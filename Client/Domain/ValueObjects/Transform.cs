using Client.Domain.Common;

namespace Client.Domain.ValueObjects
{
    public class Transform : ObservableObject
    {
        private Vector3 position;
        private Vector3 rotation;
        private Vector3 velocity;
        private Vector3 acceleration;

        public Vector3 Position { get => position; set { if (value != position) { position = value; OnPropertyChanged("Position"); } } }
        public Vector3 Rotation { get => rotation; set { if (value != rotation) { rotation = value; OnPropertyChanged("Rotation"); } } }
        public Vector3 Velocity { get => velocity; set { if (value != velocity) { velocity = value; OnPropertyChanged("Velocity"); } } }
        public Vector3 Acceleration { get => acceleration; set { if (value != acceleration) { acceleration = value; OnPropertyChanged("Acceleration"); } } }

        public Transform(Vector3 position, Vector3 rotation, Vector3 velocity, Vector3 acceleration)
        {
            this.position = position;
            this.rotation = rotation;
            this.velocity = velocity;
            this.acceleration = acceleration;
        }
    }
}
