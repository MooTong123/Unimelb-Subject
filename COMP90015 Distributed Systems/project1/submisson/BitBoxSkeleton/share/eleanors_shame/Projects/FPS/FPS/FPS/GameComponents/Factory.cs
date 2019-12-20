using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

using FPS.Blocks;
using FPS.Entities;

namespace FPS.GameComponents
{
    public static class Factory
    {
        public static Collidable Make(System.Drawing.Color col, Microsoft.Xna.Framework.Game game, Microsoft.Xna.Framework.Vector3 position)
        {
            int c = col.ToArgb();
            if (c == Color.Red.ToArgb()) {
                return new Dirt(game, position);
            }
            if (c == Color.Blue.ToArgb()) {
                return new Stone(game, position);
            }
            if (c == Color.Lime.ToArgb()) {
                return new Mushroom(game, position);
            }
            if (c == Color.Black.ToArgb()) {
                return null;
            }
            throw new NotImplementedException();
        }
    }
}
