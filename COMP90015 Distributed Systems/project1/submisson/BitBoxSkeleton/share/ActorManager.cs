using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;

namespace _3D
{
    public class ActorManager : GameComponent
    {
        List<Actor> actors = new List<Actor>();
        Game1 game;

        public ActorManager(Game game)
            : base(game)
        {
            this.game = game as Game1;
        }

        public void Add(Actor actor)
        {
            actors.Add(actor);
        }

        public override void Update(GameTime gameTime)
        {
            foreach (Actor a in actors)
            {
                a.Update();
            }
        }
    }
}
