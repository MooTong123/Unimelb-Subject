using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Items
{
    public class Mush: Item
    {
        public Mush(Game game, Vector3 position)
            : base(game, new InventoryItems.Mush(game), game.Content.Load<Texture2D>("item_mushroom"), position)
        { }
    }
}
