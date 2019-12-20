using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace physics
{
    public class Body
    {
        Game1 game;

        // linear

        protected Vector2 position; // in m
        protected Vector2 momentum; // in kgms-1

        protected float size; // in m from the origin i.e. 16m = 32x32 square
        protected float mass; // in kg

        public Vector2 Position { get { return position; } }
        
        // secondary values
        public Vector2 Velocity { get { return (mass == 0)?(Vector2.Zero):(momentum / mass); } } // in ms-1
        Vector2 force; // in N

        // angular 

        protected float inertia; // in kgm^2
        protected float torque; // in Nm
        protected float angularMomentum; // in Nms
        protected float rotation;

        public float Rotation { get { return rotation; } }

        public Rectangle BoundingRectangle
        {
            get
            {
                return new Rectangle((int)position.X, (int)position.Y,
                                     (int)(2 * size * (Math.Sin(rotation) + Math.Cos(rotation))),
                                     (int)(2 * size * (Math.Sin(rotation) + Math.Cos(rotation))));
            }
        }
        
        public Body(Game1 game, Vector2 position, float mass, float size)
        {
            this.game = game;
            this.position = position;
            this.mass = mass;
            this.size = size;

            inertia = mass * size * size;
        }
        public Body(Body copy)
        {
            this.angularMomentum = copy.angularMomentum;
            this.force = copy.force;
            this.game = copy.game;
            this.inertia = copy.inertia;
            this.mass = copy.mass;
            this.momentum = copy.momentum;
            this.position = copy.position;
            this.rotation = copy.rotation;
            this.size = copy.size;
            this.torque = copy.torque;
        }

        public virtual void Push(Vector2 force)
        {
            // simply apply a force to the mass
            // force in N
            if (mass == 0) return;
            this.force += force;
        }
        public void CounterPush(float amount)
        {
            if (mass == 0) return;
            // apply a force against the movement for approximating friction etc.
            // amount is a ratio i.e. N/N
            //
            // setting amount to 1 will result in the body taking exactly as long to speed up
            // as it does to slow down
            // setting it higher will reduce the maximum speed reached by the body

            Vector2 push = Velocity;

            // if the velocity is zero do nothing
            if (push == Vector2.Zero)
            {
                return;
            }

            // negate the vector and shrink it
            push = -push;
            push *= amount;

            Push(push);

            // do torque
            ApplyTorque(-angularMomentum * amount);
        }

        public void Update(GameTime time)
        {
            if (mass == 0) return; 

            float dt = time.ElapsedGameTime.Milliseconds / 1000f;
            // integrate the body one step forward and test for collisions
            /*Body copy = new Body(this);
            copy.Integrate(dt);
            Body collision = game.World.FindCollision(copy, this);
            if (collision != null)
            {
                float stepSize = 0.0001f;
                copy = new Body(this);
                float timeStep = 0f;
                do
                {
                    timeStep += stepSize;
                    copy.Integrate(stepSize);
                    collision = game.World.FindCollision(copy, this);
                }
                while (collision == null);
                Vector2 depth = FindMinimumTranslation(copy, collision);
                // v = 2s/t - u
                // i.e. delta v = 2s/t - 2u
                // and delta v * m / t = F
                Vector2 deltaVelocity = 2 * depth / timeStep - 2 * Velocity;
                Push(deltaVelocity * mass / timeStep);
            }*/

            // integrate using seconds as a decimal to keep units to scale
            // (GameTime.TotalGameTime.Milliseconds is an int)
            Integrate(dt);
            force = Vector2.Zero;
            torque = 0;
            Body collision = game.World.FindCollision(this);
            if (collision != null)
            {
                Vector2 depth = Vector2.Zero;
                List<Body> tested = new List<Body>();
                do
                {
                    tested.Add(collision);
                    depth += FindMinimumTranslation(this, collision);
                    collision = game.World.FindCollision(this, tested);
                }
                while (collision != null);
                position += depth;
                momentum = Vector2.Zero;
            }
        }

        public void Integrate(float dt)
        {
            LinearIntegrate(dt);
            AngularIntegrate(dt);
        }

        protected struct LinearDerivative
        {
            public Vector2 dx;
            public Vector2 dp;
        }
        protected LinearDerivative LinearEvaluate(float dt, LinearDerivative d)
        {
            // approximate an Euler solution 
            LinearDerivative output = new LinearDerivative();
            output.dx = Velocity + d.dp * dt / mass;   // dx/dt = v
            output.dp = force;           // dv/dt = a = f/m
            return output;
        }
        protected void LinearIntegrate(float dt)
        {
            // using the Runge-Kutta(4) (RK4) method
            // http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

            // evaluate successive derivatives to approximate the curve
            // given k1 = dy/dt = f(t,y):
            // k1 = f(t(n), y(n))
            // k2 = f(t(n) + h/2, y(n) + h*k1/2) etc.
            LinearDerivative a = LinearEvaluate(0f, new LinearDerivative()); // derivative is undefined but multiplied by 0 (= dt) so it's fine
            LinearDerivative b = LinearEvaluate(dt * 0.5f, a);
            LinearDerivative c = LinearEvaluate(dt * 0.5f, b);
            LinearDerivative d = LinearEvaluate(dt, c);

            // Runge-Kutta: y(n+1) = y(n) + h/6(k1 + 2k2 + 2k3 + k4)
            // h > 0 is the step size i.e. t(n+1) = n + h
            Vector2 dxdt = 1f / 6f * (a.dx + 2f * (b.dx + c.dx) + d.dx);
            Vector2 dpdt = 1f / 6f * (a.dp + 2f * (b.dp + c.dp) + d.dp);

            position += dxdt * dt;
            momentum += dpdt * dt;
        }

        public void ApplyTorque(float amount)
        {
            torque += amount;
        }

        protected struct AngularDerivative
        {
            public float spin;
            public float torque;
        }
        protected AngularDerivative AngularEvaluate(float dt, AngularDerivative d)
        {
            // approximate an Euler solution 
            // spin is roughly analogous to position
            AngularDerivative output = new AngularDerivative();
            output.spin = angularMomentum + d.torque * dt / inertia;
            output.torque = torque;
            return output;
        }
        protected void AngularIntegrate(float dt)
        {
            // using the Runge-Kutta(4) (RK4) method
            // http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods

            // evaluate successive derivatives to approximate the curve
            // given k1 = dy/dt = f(t,y):
            // k1 = f(t(n), y(n))
            // k2 = f(t(n) + h/2, y(n) + h*k1/2) etc.
            AngularDerivative a = AngularEvaluate(0f, new AngularDerivative()); // derivative is undefined but multiplied by 0 (= dt) so it's fine
            AngularDerivative b = AngularEvaluate(dt * 0.5f, a);
            AngularDerivative c = AngularEvaluate(dt * 0.5f, b);
            AngularDerivative d = AngularEvaluate(dt, c);

            // Runge-Kutta: y(n+1) = y(n) + h/6(k1 + 2k2 + 2k3 + k4)
            // h > 0 is the step size i.e. t(n+1) = n + h
            float spin = 1f / 6f * (a.spin + 2f * (b.spin + c.spin) + d.spin);
            float torque = 1f / 6f * (a.torque + 2f * (b.torque + c.torque) + d.torque);

            rotation += spin * dt / inertia;
            angularMomentum += torque * dt;
        }

        protected Vector2 FindMinimumTranslation(Body body, Body collision)
        {
            Vector2 ret = new Vector2();
            ret = body.position - collision.position;
            float length = body.size + collision.size;
            float changeInLength = length - ret.Length();
            ret.Normalize();
            ret *= Math.Abs(changeInLength);
            return ret;
        }
    }
}
