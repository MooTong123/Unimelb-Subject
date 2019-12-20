using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace _3D
{
    public class Actor : Object
    {
        Vector3 position;
        Vector3 velocity;
        Vector3 acceleration;
        Vector3 size;
        Game1 game;
        BoundingBox bb;

        float yRot = 0f;
        float torque = 0f;
        float angularMomentum = 0f;

        float friction = 1f;

        readonly float mass;

        public Vector3 Position { get { return position; } }
        public Vector3 Velocity { get { return velocity; } }
        public float Rotation { get { return yRot; } }
        public BoundingBox BoundingBox { get { return bb; } }

        public Actor(Game game, Vector3 pos, Vector3 size, float mass = 1f)
        {
            this.game = game as Game1;
            position = pos;
            this.size = size;

            this.game.ActorManager.Add(this);

            bb = new BoundingBox(pos, pos + size);

            this.mass = mass;
        }

        public void Push(Vector3 force)
        {
            acceleration += force / mass;
        }
        public void ApplyTorque(float torque)
        {
            this.torque += torque;
        }

        private void UpdateBoundingBox()
        {
            bb = new BoundingBox(position - size / 2, position + size / 2);
        }

        public void Update()
        {
            angularMomentum += torque / mass;
            yRot += angularMomentum;

            velocity += acceleration;
            position += Vector3.Transform(velocity, Matrix.CreateRotationY(yRot));
            
            velocity /= friction;
            angularMomentum /= friction;

            acceleration = Vector3.Zero;
            torque = 0;

            UpdateBoundingBox();
        }
    }
}
