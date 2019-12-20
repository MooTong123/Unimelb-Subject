using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Entity
{
    public class Mushroom: Entity
    {
        Texture2D tex_hurt;
        Texture2D tex_mush;

        double hit_time = 0;
        double t = 0;

        public Mushroom(Game game, int id, Vector3 position)
            : base(game, id, position, "mushroom", 5)
        {
            tex_mush = texture;
            tex_hurt = game.Content.Load<Texture2D>("mushroom_hurt");
        }

        public override bool hurt(int amount = 1)
        {
            if (texture == tex_hurt)
            {
                return false;
            }

            hit_time = t;

            texture = tex_hurt;
            health -= amount;
            if (health <= 0)
            {
                alive = false;
                return true;
            }
            return false;
        }

        public override void Update(GameTime gameTime)
        {
            t = gameTime.TotalGameTime.TotalMilliseconds;
            if (t - hit_time > 700)
            {
                texture = tex_mush;
            }
            base.Update(gameTime);
        }
    }
}
