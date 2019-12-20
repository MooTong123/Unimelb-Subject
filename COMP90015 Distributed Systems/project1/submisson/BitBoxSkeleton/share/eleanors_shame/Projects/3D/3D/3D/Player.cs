using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace _3D
{
    public class Player: DrawableGameComponent
    {
        ModelContainer cube;
        Actor actor;
        Game1 game;
        const float speed = 0.5f;
        const float rotSpeed = speed / 7.5f;
        const float strafeSpeed = speed / 1.5f;

        int rotating = 0;
        int strafing = 0;

        bool targeting = false;

        public Vector3 Position { get { return actor.Position; } }
        public Vector3 Velocity { get { return actor.Velocity; } }

        public Player(Game game)
            : base(game)
        {
            this.game = game as Game1;

            actor = new Actor(this.game, Vector3.Zero, new Vector3(32f));

            this.game.Camera.SetTarget(actor);
        }

        protected override void LoadContent()
        {
            cube = new ModelContainer(game, "Models/cube");
        }

        public override void Update(GameTime gameTime)
        {
            if (Input.keyDown(Keys.RightShift))
            {
                targeting = true;
                if (rotating != 0)
                {
                    actor.ApplyTorque(rotSpeed * rotating);
                    rotating = 0;
                }
                if (Input.State.IsKeyDown(Keys.A))
                {
                    actor.Push(new Vector3(strafeSpeed, 0, 0));
                    strafing = -1;
                }
                if (Input.State.IsKeyDown(Keys.D))
                {
                    actor.Push(new Vector3(-strafeSpeed, 0, 0));
                    strafing = 1;
                }
            }

            if (Input.keyUp(Keys.RightShift))
            {
                targeting = false;
                if (strafing != 0)
                {
                    actor.Push(new Vector3(strafeSpeed * strafing, 0, 0));
                    strafing = 0;
                }
                if (Input.State.IsKeyDown(Keys.A))
                {
                    actor.ApplyTorque(rotSpeed);
                    rotating = -1;
                }
                if (Input.State.IsKeyDown(Keys.D))
                {
                    actor.ApplyTorque(-rotSpeed);
                    rotating = 1;
                }

                // needed to stop events registering twice
                if (Input.keyUp(Keys.A))
                {
                    actor.ApplyTorque(rotSpeed);
                }
                if (Input.keyUp(Keys.D))
                {
                    actor.ApplyTorque(-rotSpeed);
                }
            }

            if (Input.keyDown(Keys.W))
            {
                actor.Push(new Vector3(0, 0, speed));
            }
            if (Input.keyDown(Keys.S))
            {
                actor.Push(new Vector3(0, 0, -speed));
            }
            if (Input.keyUp(Keys.W))
            {
                actor.Push(new Vector3(0, 0, -speed));
            }
            if (Input.keyUp(Keys.S))
            {
                actor.Push(new Vector3(0, 0, speed));
            }
            if (!targeting)
            {
                if (Input.keyDown(Keys.A))
                {
                    actor.ApplyTorque(rotSpeed);
                    rotating = -1;
                }
                if (Input.keyDown(Keys.D))
                {
                    actor.ApplyTorque(-rotSpeed);
                    rotating = 1;
                }
                if (Input.keyUp(Keys.A))
                {
                    actor.ApplyTorque(-rotSpeed);
                    rotating = 0;
                }
                if (Input.keyUp(Keys.D))
                {
                    actor.ApplyTorque(rotSpeed);
                    rotating = 0;
                }
            }
            else
            {
                if (Input.keyDown(Keys.A))
                {
                    actor.Push(new Vector3(strafeSpeed, 0, 0));
                    strafing = -1;
                }
                if (Input.keyDown(Keys.D))
                {
                    actor.Push(new Vector3(-strafeSpeed, 0, 0));
                    strafing = 1;
                }
                if (Input.keyUp(Keys.A))
                {
                    actor.Push(new Vector3(-strafeSpeed, 0, 0));
                    strafing = 0;
                }
                if (Input.keyUp(Keys.D))
                {
                    actor.Push(new Vector3(strafeSpeed, 0, 0));
                    strafing = 0;
                }
            }

            game.Camera.SetView();
        }

        public override void Draw(GameTime gameTime)
        {
            cube.Draw(actor.Position, Matrix.CreateRotationY(actor.Rotation));
        }
    }
}
