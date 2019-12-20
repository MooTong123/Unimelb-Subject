using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

// TODO: implement jumping
namespace Zelda2D
{
    public class Player : DrawableGameComponent
    {
        Game1 game;
        Sprite sprite;
        Sprite shadow;
        Actor actor;
        const float speed = 4f;
        bool onGround = true;
        float z = 0f;

        public Player(Game game)
            : base(game)
        {
            this.game = game as Game1;
            actor = new Actor(game, new Vector2(128, 128), new Vector2(32, 32));
        }

        public override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            sprite = new Sprite(game, "circle");
            shadow = new Sprite(game, "shadow");
            base.LoadContent();
        }

        private void HandleMovement()
        {
            // don't let the player freely manoeveur in air
            if (onGround)
            {
                if (Input.State.IsKeyDown(Keys.Left) && actor.Velocity.X == 0)
                {
                    actor.Push(new Vector2(-speed, 0));
                }
                if (Input.State.IsKeyDown(Keys.Up) && actor.Velocity.Y == 0)
                {
                    actor.Push(new Vector2(0, -speed));
                }
                if (Input.State.IsKeyDown(Keys.Right) && actor.Velocity.X == 0)
                {
                    actor.Push(new Vector2(speed, 0));
                }
                if (Input.State.IsKeyDown(Keys.Down) && actor.Velocity.Y == 0)
                {
                    actor.Push(new Vector2(0, speed));
                }
                
                // accounting for diagonal movement :/
                if (Input.keyUp(Keys.Left) || Input.keyUp(Keys.Right))
                {
                    Vector2 vel = actor.Velocity;
                    actor.Push(new Vector2(-vel.X, 0));
                    // there will be a difference between speed and actor.Velocity.Y if there was diagonal movement
                    // only change actor.Velocity.Y if it was moving at all
                    if (vel.Y > 0)
                    {
                        actor.Push(new Vector2(0, speed - vel.Y));
                    }
                    if (vel.Y < 0)
                    {
                        actor.Push(new Vector2(0, -speed - vel.Y));
                    }

                    // update in case both directions are released at the same time
                    actor.Update();
                }
                if (Input.keyUp(Keys.Up) || Input.keyUp(Keys.Down))
                {
                    Vector2 vel = actor.Velocity;
                    actor.Push(new Vector2(0, -vel.Y));
                    if (vel.X > 0)
                    {
                        actor.Push(new Vector2(speed - vel.X, 0));
                    }
                    if (vel.X < 0)
                    {
                        actor.Push(new Vector2(-speed - vel.X, 0));
                    }

                    actor.Update();
                }

                // quick hack to fix diagonal speeds
                if (actor.Velocity.Length() > speed && Math.Abs(actor.Velocity.X) > 0 && Math.Abs(actor.Velocity.Y) > 0)
                {
                    // pythagoras, this is what each axis should be
                    // subtract it from speed to get the amount needed
                    // to adjust velocity by
                    float overSpeed = speed - (float)Math.Sqrt(speed * 2);

                    // get signs right
                    Vector2 adjust = new Vector2(overSpeed, overSpeed);
                    adjust.X *= Math.Sign(actor.Velocity.X);
                    adjust.Y *= Math.Sign(actor.Velocity.Y);

                    actor.Push(-adjust);
                }
            }
        }
        public override void Update(GameTime gameTime)
        {
            HandleMovement();
            if (!onGround)
            {
                actor.Push(new Vector2(0, -z));
                actor.Update();
            }
            Console.WriteLine(actor.Position);
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            Vector2 adjustedPosition = actor.Position;
            if (!onGround)
            {
                shadow.Draw(actor.Position);

                adjustedPosition.Y -= z;
            }
            sprite.Draw(actor.Position);
        }
    }
}
