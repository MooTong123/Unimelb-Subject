using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace FPS
{
    public static class Global
    {
        public const int TileSize = 16;
        public const bool Write = true;
        public const int ChunkSize = 4;
        public static Random rand = new Random();
        public static Vector3 randomVector(float n = 3)
        {
            Vector3 ret = new Vector3();
            ret.X = (float)Global.rand.NextDouble() * n - n / 2;
            ret.Y = (float)Global.rand.NextDouble() * n - n / 2;
            ret.Z = (float)Global.rand.NextDouble() * n - n / 2;
            return ret;
        }
    }
}
