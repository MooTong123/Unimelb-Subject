using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace physics
{
    public class Block: DrawableGameComponent
    {
        Sprite sprite;
        Body body;
        Game1 game;

        public Block(Game1 game, Vector2 position)
            : base(game)
        {
            this.game = game;
            body = new Body(game, position, 1f, 16f);
            sprite = new Sprite(game, "square32");
            game.Components.Add(this);
            game.World.AddBody(body);
        }

        public override void Draw(GameTime gameTime)
        {
            sprite.Draw(body.Position, body.Rotation);
        }
    }
}
