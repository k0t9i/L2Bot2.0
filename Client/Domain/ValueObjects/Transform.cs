namespace Client.Domain.ValueObjects
{
    public class Transform
    {
        public Vector3 Position { get; set; }
        public Vector3 Rotation { get; set; }
        public Vector3 Velocity { get; set; }
        public Vector3 Acceleration { get; set; }

        public Transform(Vector3 position, Vector3 rotation, Vector3 velocity, Vector3 acceleration)
        {
            Position = position;
            Rotation = rotation;
            Velocity = velocity;
            Acceleration = acceleration;
        }
    }
}
