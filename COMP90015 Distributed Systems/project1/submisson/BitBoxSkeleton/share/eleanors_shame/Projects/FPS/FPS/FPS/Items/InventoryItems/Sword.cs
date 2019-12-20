using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Items.InventoryItems
{
    public class Sword: InventoryItem
    {
        public Sword(Game game)
            : base(game, 0)
        {
            sprite = game.Content.Load<Texture2D>("item_sword");
        }
        public override void use()
        {
        }
    }
}
