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


namespace Physics
{
    public class Box: DrawableGameComponent
    {
        Game game;
        RigidBody body;
        Sprite sprite;

        bool control;
        const float speed = 0.1f;
        const float rspeed = 0.005f;

        public Box(Game game, Vector2 pos, float mass, bool control = false, bool movable = true)
            : base(game)
        {
            this.game = game;
            body = new RigidBody(game, pos, new Vector2(32, 32), mass, movable);
            this.control = control;
        }

        protected override void LoadContent()
        {
            sprite = new Sprite(game as Physics, "square32");
            game.Components.Add(body);
        }

        public override void Update(GameTime gameTime)
        {
            if (control)
            {
                if (Input.State.IsKeyDown(Keys.Right))
                {
                    body.Push(new Vector2(speed, 0));
                }
                if (Input.State.IsKeyDown(Keys.Left))
                {
                    body.Push(new Vector2(-speed, 0));
                }
                if (Input.State.IsKeyDown(Keys.Up))
                {
                    body.Push(new Vector2(0, -speed));
                }
                if (Input.State.IsKeyDown(Keys.Down))
                {
                    body.Push(new Vector2(0, speed));
                }

                if (Input.State.IsKeyDown(Keys.W))
                {
                    body.Rotate(rspeed);
                }
                if (Input.State.IsKeyDown(Keys.S))
                {
                    body.Rotate(-rspeed);
                }
            }
        }

        public override void Draw(GameTime gameTime)
        {
            sprite.Draw(body.Position + body.Velocity * (gameTime.ElapsedGameTime.Milliseconds / 64), body.Rot);
        }
    }
}
