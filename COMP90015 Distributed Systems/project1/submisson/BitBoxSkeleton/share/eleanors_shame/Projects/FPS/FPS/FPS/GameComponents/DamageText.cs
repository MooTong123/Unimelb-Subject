using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.GameComponents
{
    public class DamageText: DrawableGameComponent
    {
        public enum Units
        {
            Player,
            Target
        }
        Units position;

        FirstPerson game;
        SpriteFont font;
        string number;

        int y = 398;
        float t = 0;
        bool draw = true;

        public DamageText(Game game, int amount, Units target)
            : base(game)
        {
            position = target;
            number = (-amount).ToString();
            this.game = game as FirstPerson;
            game.Components.Add(this);

            font = game.Content.Load<SpriteFont>("damage");
        }

        public override void Update(GameTime gameTime)
        {
            y--;
            t++;
            if (t > 100)
            {
                draw = false;
                game.Components.Remove(this);
            }
        }
        public override void Draw(GameTime gameTime)
        {
            if (!draw) return;
            game.spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.AlphaBlend);
            game.spriteBatch.DrawString(font, number,
                                        new Vector2((position == Units.Player) ? (32) : (800 - 32 - font.MeasureString(number).X), y),
                                        Color.Red * ((100 - t) / 100));
            game.spriteBatch.End();
            game.GraphicsDevice.BlendState = BlendState.Opaque;
            game.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
        }
    }
}
