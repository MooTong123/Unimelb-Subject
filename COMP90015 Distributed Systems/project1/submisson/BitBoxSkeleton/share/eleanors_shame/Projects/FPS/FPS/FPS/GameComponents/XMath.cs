using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace FPS.GameComponents
{
    public static class XMath
    {
        public static Vector3 Mean(Vector3 a, Vector3 b)
        {
            return (a + b) / new Vector3(2);
        }
    }
}
