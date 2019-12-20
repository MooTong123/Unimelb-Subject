using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using FPS.GameComponents;

namespace FPS.Effects
{
    public class Effect: DrawableGameComponent
    {
        protected Unit target;
        protected FirstPerson game;
        protected Damage type;
        protected Texture2D icon;

        private double startTime;
        private double duration;
        private double time;

        private Vector2 pos;

        protected double Time
        {
            get { return time; }
        }

        private const int size = 32;

        public Damage Type
        {
            get { return type; }
        }
        public Effect(Game game, double duration)
            : base(game)
        {
            this.game = game as FirstPerson;
            this.duration = duration;

            pos = new Vector2(10, 10);
        }

        public void apply(Unit target)
        {
            this.target = target;
            startTime = game.Time.TotalGameTime.TotalMilliseconds;
            game.Components.Add(this);
        }

        public override void Update(GameTime gameTime)
        {
            time = gameTime.TotalGameTime.TotalMilliseconds - startTime;
            if (time >= duration)
            {
                game.Components.Remove(this);
                return;
            }
            effect();
        }

        public virtual void effect()
        {
        }

        public override void Draw(GameTime gameTime)
        {
            if (icon != null)
            {
                game.spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.AlphaBlend);
                game.spriteBatch.Draw(icon, new Rectangle((int)pos.X, (int)pos.Y, size, size), Color.White);
                game.spriteBatch.End();
                game.GraphicsDevice.BlendState = BlendState.Opaque;
                game.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
            }
        }
    }
}
