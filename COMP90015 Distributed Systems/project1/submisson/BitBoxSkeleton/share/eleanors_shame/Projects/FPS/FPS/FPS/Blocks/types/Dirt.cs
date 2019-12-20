using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Blocks
{
    public class Dirt: Block
    {
        public Dirt(Game game, Vector3 Position)
            : base(game, Position, "dirt")
        { }
    }
}
