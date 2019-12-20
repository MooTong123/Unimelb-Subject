using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Items.InventoryItems
{
    public abstract class InventoryItem
    {
        protected Texture2D sprite;

        protected FirstPerson game;

        protected int id;

        public int Id
        {
            get { return id; }
        }
        public int count
        {
            get;
            set;
        }

        public InventoryItem(Game game, int id)
        {
            this.game = game as FirstPerson;
            this.id = id;
            count = 0;
        }

        public void draw(int slot)
        {
            //game.spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.AlphaBlend);

            game.spriteBatch.Draw(sprite, new Rectangle(282 + slot * 48, 430, 48, 48), Color.White);

            //game.spriteBatch.End();
            //game.GraphicsDevice.BlendState = BlendState.Opaque;
            //game.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
        }
        public abstract void use();
    }
}
