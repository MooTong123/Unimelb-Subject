using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Items.InventoryItems
{
    public class Mush: InventoryItem
    {
        public Mush(Game game)
            : base(game, 1)
        {
            sprite = game.Content.Load<Texture2D>("item_mushroom");
        }
        public override void use()
        {
            if (count > 0)
            {
                game.player.heal(15);
                count--;
            }
        }
    }
}
