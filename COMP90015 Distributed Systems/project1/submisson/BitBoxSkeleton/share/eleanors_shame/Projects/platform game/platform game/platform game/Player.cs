using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FarseerPhysics.Factories;
using FarseerPhysics.Dynamics;

using Microsoft.Xna.Framework;

namespace platform_game
{
    public class Player: DrawableGameComponent
    {
        Body body;
        Game1 game;
        Sprite sprite;

        public Player(Game1 game, Vector2 pos)
            : base(game)
        {
            this.game = game;
            body = BodyFactory.CreateRectangle(game.World, 32, 32, 1);
            body.BodyType = BodyType.Dynamic;
            body.Position = pos;

            sprite = new Sprite(game, "square32");
            game.Components.Add(this);
        }

        public override void Update(GameTime gameTime)
        {
            
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            sprite.Draw(body.Position);
        }
    }
}
