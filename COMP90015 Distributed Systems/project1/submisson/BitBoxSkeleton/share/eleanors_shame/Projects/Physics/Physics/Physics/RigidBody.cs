using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Physics
{
    public class RigidBody: GameComponent
    {
        Vector2 x;
        Vector2 size;
        float mass;
        Vector2 acceleration;
        Vector2 velocity;
        Vector2 force;

        double torque;
        double rot;
        double rotAccel;
        double rotVeloc;
        double inertia;

        BoundingBox bb;
        public BoundingBox Box { get { return bb; } }

        public Vector2 Position { get { return x; } }
        public Vector2 Velocity { get { return velocity; } }
        public Vector2 Acceleration { get { return acceleration; } }
        public float Mass { get { return mass; } }
        public double Rot { get { return rot; } }

        bool movable;

        static List<RigidBody> bodies = new List<RigidBody>();
        public RigidBody(Game game, Vector2 pos, Vector2 size, float mass, bool movable)
            : base(game)
        {
            this.mass = mass;
            this.movable = movable;
            x = pos;
            this.size = size;
            bodies.Add(this);

            rot = 0;
            torque = 0;
            inertia = 1;

            UpdateBox();
        }
        void UpdateBox()
        {
            bb.Min = new Vector3(x, 0);
            bb.Max = new Vector3(x + size, 0);
        }

        public void Push(Vector2 Force)
        {
            force += Force;
        }

        public void Rotate(double amt)
        {
            torque += amt;
        }

        public override void Update(GameTime gameTime)
        {
            if (movable)
            {
                // update variables
                acceleration = force / mass;
                if (mass < 10000)
                {
                    bool onGround = false;
                    foreach (RigidBody body in bodies)
                    {
                        BoundingBox b = bb;
                        b.Max.Y += 1;
                        if (body != this && body.movable == false && b.Intersects(body.bb))
                        {
                            onGround = true;
                        }
                    }
                    if (!onGround)
                    {
                        acceleration.Y += 0.098f;
                    }
                }
                velocity += acceleration * (gameTime.ElapsedGameTime.Milliseconds / 16);
                x += velocity * (gameTime.ElapsedGameTime.Milliseconds / 16);
                Vector2 posPrev = x;
                UpdateBox();


                rotAccel = torque / inertia;
                rotVeloc += rotAccel * (gameTime.ElapsedGameTime.Milliseconds / 16);
                rot += rotVeloc * (gameTime.ElapsedGameTime.Milliseconds / 16); 

                // check collision
                foreach (RigidBody body in bodies)
                {
                    x += velocity * (gameTime.ElapsedGameTime.Milliseconds / 16);
                    UpdateBox();

                    OBB myBox = new OBB(x + size / 2, (float)(rot * (Math.PI / 180)), size / 2);
                    OBB theirBox = new OBB(body.Position + body.size / 2, (float)(body.Rot * (Math.PI / 180)), body.size / 2);
                    if (myBox.Intersects(theirBox) && this != body)
                    {
                        Vector2 vi = velocity;
                        x = posPrev;

                        velocity = (velocity * (mass - body.Mass) + 2 * body.mass * body.velocity) / (mass + body.Mass);
                        body.velocity = (body.velocity * (body.mass - mass) + 2 * mass * vi) / (mass + body.mass);

                        if (x.X > body.Position.X)
                        {
                            float difference = x.X - body.x.X;
                            body.Rotate(difference / 10000);
                        }
                        else if (x.X < body.Position.X)
                        {
                            float difference = body.x.X - x.X;
                            body.Rotate(-difference / 10000);
                        }

                        BoundingBox b = bb;
                        b.Max.X += 32;
                        if (b.Intersects(body.Box))
                        {
                            velocity.Y = -velocity.Y;
                        }
                        b.Max.X -= 64;
                        if (b.Intersects(body.Box))
                        {
                            velocity.Y = -velocity.Y;
                        }

                        b.Max.X += 32;
                        b.Max.Y += 32;
                        if (b.Intersects(body.Box))
                        {
                            velocity.X = -velocity.X;
                        }
                        b.Max.Y -= 64;
                        if (b.Intersects(body.Box))
                        {
                            velocity.X = -velocity.X;
                        }

                        x += velocity * (gameTime.ElapsedGameTime.Milliseconds / 16);
                        if (body.movable)
                        {
                            body.x += body.velocity * (gameTime.ElapsedGameTime.Milliseconds / 16);

                            body.UpdateBox();
                        }
                    }
                    else
                    {
                        x = posPrev;
                    }
                }
                UpdateBox();

                // reset
                force.X = 0;
                force.Y = 0;
                torque = 0;
            }
        }
        
    }
}
