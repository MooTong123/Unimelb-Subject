using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Blocks
{
    public class Stone: Block
    {
        public Stone(Game game, Vector3 Position)
            : base(game, Position, "stone")
        { }
    }
}
