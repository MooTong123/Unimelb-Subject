using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace physics
{
    public class World : GameComponent
    {
        List<Body> bodies = new List<Body>();
        Game1 game;
        public World(Game1 game): base(game)
        {
            this.game = game;
        }

        public void AddBody(Body body)
        {
            bodies.Add(body);
        }

        public override void Update(GameTime gameTime)
        {
            foreach (Body b in bodies)
            {
                // world friction
                b.CounterPush(1f);

                b.Update(gameTime);
            }
        }

        public Body FindCollision(Body caller, Body ignoreThisBody = null)
        {
            foreach (Body b in bodies)
            {
                if (ignoreThisBody != null)
                {
                    if (b != caller && b != ignoreThisBody && caller.BoundingRectangle.Intersects(b.BoundingRectangle))
                    {
                        return b;
                    }
                }
                else if (b != caller && caller.BoundingRectangle.Intersects(b.BoundingRectangle))
                {
                    return b;
                }
            }
            return null;
        }
        public Body FindCollision(Body caller, List<Body> ignoreTheseBodies)
        {
            foreach (Body b in bodies)
            {
                if (b != caller && caller.BoundingRectangle.Intersects(b.BoundingRectangle))
                {
                    bool covered = false;
                    foreach (Body other in ignoreTheseBodies)
                    {
                        if (b == other)
                        {
                            covered = true;
                        }
                    }
                    if (!covered)
                    {
                        return b;
                    }
                }
            }
            return null;
        }
    }
}
