using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Physics
{
    public class Sprite
    {
        private Texture2D spriteTexture;
        private Physics game;

        public Sprite(Physics game, String filename)
        {
            spriteTexture = game.Content.Load<Texture2D>(filename);
            this.game = game;
        }
         
        public void Draw(Vector2 position, double rot = 0)
        {
            game.SpriteBatch().Draw(spriteTexture, position + new Vector2(spriteTexture.Width / 2, spriteTexture.Height / 2), null, Color.White, (float)rot,
                                    new Vector2(spriteTexture.Width/2, spriteTexture.Height/2),
                                    1.0f, SpriteEffects.None, 0f);
        }
    }
}
