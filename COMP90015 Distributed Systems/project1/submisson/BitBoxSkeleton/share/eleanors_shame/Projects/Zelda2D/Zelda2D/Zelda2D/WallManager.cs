using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace Zelda2D
{
    public class WallManager : DrawableGameComponent
    {
        List<Wall> walls = new List<Wall>();
        Game1 game;

        Sprite sprite;

        public WallManager(Game game)
            : base(game)
        {
            this.game = game as Game1;
        }

        protected override void LoadContent()
        {
            sprite = new Sprite(this.game, "box");
        }

        public int Add(Wall wall)
        {
            walls.Add(wall);
            game.QuadTree.Add(wall);
            return walls.Count - 1;
        }

        public Wall CheckIntersection(BoundingBox bb)
        {
            Rectangle actorRectangle = new Rectangle((int)bb.Min.X, (int)bb.Min.Y,
                                                     (int)bb.Max.X - (int)bb.Min.X, (int)bb.Max.Y - (int)bb.Min.Y);
            Object[] objects = game.QuadTree.GetItems(actorRectangle);
            Console.WriteLine(objects.Count());
            foreach (Object o in objects)
            {
                if (o.GetType() == Types.Wall)
                {
                    Rectangle otherRectangle = o.GetPosition();
                    if (actorRectangle.Intersects(otherRectangle))
                    {
                        return walls[o.GetID()];
                    }
                }
            }
            return null;
        }

        public override void Draw(GameTime gameTime)
        {
            foreach (Wall w in walls)
            {
                sprite.Draw(new Vector2(w.BoundingBox.Min.X, w.BoundingBox.Min.Y));
            }
        }
    }
}
