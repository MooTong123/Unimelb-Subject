using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace physics
{
    public class ControllableBlock: DrawableGameComponent
    {
        Sprite sprite;
        Body body;
        Game1 game;

        public ControllableBlock(Game1 game, Vector2 position)
            : base(game)
        {
            this.game = game;
            body = new Body(game, position, 1f, 16f);
            sprite = new Sprite(game, "square32");
            game.Components.Add(this);
            game.World.AddBody(body);
        }

        public override void Update(GameTime gameTime)
        {
            // the following is just testing fluff
            float force = 1000f; // in newtons
            if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Right))
            {
                body.Push(new Vector2(force, 0));
            }
            if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Left))
            {
                body.Push(new Vector2(-force, 0));
            }
            if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Up))
            {
                body.Push(new Vector2(0, -force));
            }
            if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Down))
            {
                body.Push(new Vector2(0, force));
            }
            if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.A))
            {
                body.ApplyTorque(-force);
            }
            if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.D))
            {
                body.ApplyTorque(force);
            }
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            sprite.Draw(body.Position, body.Rotation);
        }
    }
}
