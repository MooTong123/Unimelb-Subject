using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Zelda2D
{
    public class Actor: Object
    {
        Vector2 position;
        Vector2 prevPosition;
        Vector2 velocity;
        Vector2 acceleration;
        Vector2 size;
        Game1 game;
        BoundingBox bb;
        int id;

        readonly float mass;

        public Vector2 Position { get { return position; } }
        public Vector2 Velocity { get { return velocity; } }
        public BoundingBox BoundingBox { get { return bb; } }

        // the following methods are used for quad tree
        public override Rectangle GetPosition()
        {
            Rectangle ret = new Rectangle();
            ret.X = (int)position.X;
            ret.Y = (int)position.Y;
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
            return Types.Actor;
        }

        public Actor(Game game, Vector2 pos, Vector2 size, float mass = 1f)
        {
            this.game = game as Game1;
            position = pos;
            this.size = size;

            bb = new BoundingBox(new Vector3(pos, 0),
                                 new Vector3(pos + size, 1));

            this.mass = mass;

            id = this.game.ActorManager.Add(this);
        }

        public void Push (Vector2 force)
        {
            acceleration += force / mass;
        }

        public void CollisionWithActor(Actor actor)
        { }
        public Vector2 CollisionWithWall(Wall wall)
        {
            // get values, A = this, B = the wall
            Vector2 aMin = new Vector2(bb.Min.X, bb.Min.Y);
            Vector2 aMax = new Vector2(bb.Max.X, bb.Max.Y);
            Vector2 bMin = new Vector2(wall.BoundingBox.Min.X, wall.BoundingBox.Min.Y);
            Vector2 bMax = new Vector2(wall.BoundingBox.Max.X, wall.BoundingBox.Max.Y);

            // get sides
            float left = bMin.X - aMax.X;
            float right = bMax.X - aMin.X;
            float top = bMin.Y - aMax.Y;
            float bottom = bMax.Y - aMin.Y;

            // minimum translation distance
            Vector2 mtd = new Vector2();
            if (Math.Abs(left) < right)
            {
                mtd.X = left;
            }
            else
            {
                mtd.X = right;
            }

            if (Math.Abs(top) < bottom)
            {
                mtd.Y = top;
            }
            else
            {
                mtd.Y = bottom;
            }

            // find the smaller of the axes

            if (Math.Abs(mtd.X) < Math.Abs(mtd.Y))
            {
                mtd.Y = 0;
            }
            else
            {
                mtd.X = 0;
            }
            // fixes weirdness if they line up perfectly
            if (mtd == new Vector2()) mtd += new Vector2(1, 1);
            // needed to make sure they unstick properly
            position += mtd;//* 2.5f;
            UpdateBoundingBox();

            // return the impulse for possible later use
            return mtd;
        }

        private void UpdateBoundingBox()
        {
            bb = new BoundingBox(new Vector3(position, 0),
                                 new Vector3(position + size, 1));
        }

        public void Update()
        {
            prevPosition = position;
            velocity += acceleration;
            position += velocity;
            acceleration = new Vector2(0, 0);

            UpdateBoundingBox();
        }
    }
}
