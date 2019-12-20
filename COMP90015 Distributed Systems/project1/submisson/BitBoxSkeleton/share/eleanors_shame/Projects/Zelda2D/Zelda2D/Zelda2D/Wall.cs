using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Zelda2D
{
    public class Wall: Object
    {
        BoundingBox bb;
        Game1 game;

        readonly Vector2 size = new Vector2(32, 32);
        int id;

        public override Rectangle GetPosition()
        {
            Rectangle ret = new Rectangle();
            ret.X = (int)bb.Min.X;
            ret.Y = (int)bb.Min.Y;
            ret.Width = (int)size.X;
            ret.Height = (int)size.Y;

            return ret;
        }
        public override int GetID()
        {
            return id; 
        }
        public override Types GetType()
        {
            return Types.Wall;
        }

        public Vector2 Position { get { return new Vector2(bb.Min.X, bb.Min.Y); } }

        public BoundingBox BoundingBox { get { return bb; } }

        public Wall(Game game, Vector2 position)
        {
            bb.Min = new Vector3(position, 0);
            bb.Max = new Vector3(position + size, 0);
            this.game = game as Game1;
            id = this.game.WallManager.Add(this);
        }
    }
}
