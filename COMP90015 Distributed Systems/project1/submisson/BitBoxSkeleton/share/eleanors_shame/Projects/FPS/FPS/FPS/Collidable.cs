using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS
{
    public abstract class Collidable: DrawableGameComponent
    {
        protected BoundingBox bb;
        protected Texture2D texture;
        protected FirstPerson game;

        protected void update()
        {
        }

        public BoundingBox Box
        {
            get { return bb; }
        }

        public Collidable(Game game)
            : base(game)
        {
            this.game = game as FirstPerson;
        }
    }
}
