using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace FPS.Blocks
{
    public static class Factory
    {
        public static Block Create(int type, Game game, Vector3 position)
        {
            switch (type)
            {
                case 1:
                    return new Dirt(game, position);

                case 2:
                    return new Stone(game, position);

                default:
                    return null;
            }
        }
    }
}
