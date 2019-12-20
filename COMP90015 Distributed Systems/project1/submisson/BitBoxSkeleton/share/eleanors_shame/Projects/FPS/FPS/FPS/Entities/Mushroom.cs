using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using FPS.GameComponents;
using FPS.Items;

namespace FPS.Entities
{
    public class Mushroom: Entity
    {
        Texture2D tex_hurt;
        Texture2D tex_mush;

        Random rand = new Random();

        double hit_time = 0;
        double t = 0;
        double lastShot = 0;

        int offset = 0;

        bool hurting = false;

        public Mushroom(Game game, Vector3 position)
            : base(game, position, "mushroom")
        {
            name = "Mushroom";
            health = 50;
            MaxHealth = 50;
            tex_mush = texture;
            offset = rand.Next(200) + 100;
            tex_hurt = game.Content.Load<Texture2D>("mushroom_hurt");

            resist[Damage.Physical] = 50;
        }

        public override bool onHurt(float amount, Damage type)
        {
            if (hurting)
            {
                return false;
            }
            hurting = true;
            new DamageText(game, (int)amount, DamageText.Units.Target);

            hit_time = t;

            texture = tex_hurt;
            return true;
        }

        public override void Update(GameTime gameTime)
        {
            t = gameTime.TotalGameTime.TotalMilliseconds;
            if (t - hit_time > 400)
            {
                texture = tex_mush;
                hurting = false;
            }

            if (t - lastShot + offset > 800)
            {
                lastShot = t;
                new Spell(game, game.player, position);
            }
            base.update();
            base.Update(gameTime);
        }

        public override void drop()
        {
            int amount = Global.rand.Next(3) + 1;
            for (int i = 0; i < amount; ++i)
            {
                new Mush(game, position);
            }
        }
    }
}
