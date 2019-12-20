using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Zelda2D
{
    public enum Types
    {
        Actor,
        Wall
    }
    public abstract class Object
    {
        
        public abstract Rectangle GetPosition();
        public abstract int GetID();
        public abstract Types GetType();
    }
}
