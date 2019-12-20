using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Effects
{
    public class Dot: Effect
    {
        public Dot(Game game)
            : base(game, 2500)
        {
            icon = game.Content.Load<Texture2D>("eff_moosword");
        }

        public override void effect()
        {
            if (Time % 500 <= 16)
            {
                game.player.heal(1);
                target.hurt(5, Damage.Nature);
            }
        }
    }
}
