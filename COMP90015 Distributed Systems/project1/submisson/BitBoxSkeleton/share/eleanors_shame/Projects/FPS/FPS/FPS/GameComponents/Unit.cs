using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using FPS.Effects;

namespace FPS.GameComponents
{
    public abstract class Unit: Collidable
    {
        public Unit(Game game)
            : base(game)
        {
            resist = new Dictionary<Damage, Percent>();
            resist.Add(Damage.Physical, 0);
            resist.Add(Damage.Fire, 0);
            resist.Add(Damage.Frost, 0);
            resist.Add(Damage.Nature, 0);
            resist.Add(Damage.Arcane, 0);
            resist.Add(Damage.Holy, 0);
            resist.Add(Damage.Shadow, 0);
        }

        public void applyEffect(Effect effect)
        {
            if (Global.rand.Next(100) >= ((int)(float)resist[effect.Type]) * 100)
            {
                effect.apply(this);
            }
        }

        protected string name;

        public string Name
        {
            get { return name; }
        }
        public virtual float Health { get { throw new NotImplementedException(); } }

        private float health;
        public float MaxHealth
        {
            get { return health; }
            protected set { health = value; }
        }

        public abstract bool hurt(float amount, Damage type);

        protected Dictionary<Damage, Percent> resist;
    }
}
