using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Items.InventoryItems
{
    public class MooSword: InventoryItem
    {
        public MooSword(Game game)
            : base(game, 2)
        {
            sprite = game.Content.Load<Texture2D>("mooswordUpgrade");
        }
        public override void use()
        {
            game.player.replaceWeapon(new GameComponents.Player.MooSword(game.player));
        }
    }
}
