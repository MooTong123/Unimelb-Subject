using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;


namespace FPS.GameComponents
{
    public class TargetManager : Microsoft.Xna.Framework.DrawableGameComponent
    {
        private Model modelTarget;
        private FirstPerson game;
        Effect ceffect;

        List<BoundingSphere> targets = new List<BoundingSphere>();

        public TargetManager(Game parent)
            : base(parent)
        {
            game = parent as FirstPerson;

            Random rand = new Random();
            for (int i = 0; i < 20; ++i)
            {
                int x = rand.Next(game.level.Width * Global.TileSize);
                int z = rand.Next(game.level.Height * Global.TileSize);
                float y = (float)rand.Next(Global.TileSize * 4);
                float radius = (float)rand.Next(20) + 20;
            
                targets.Add(new BoundingSphere(new Vector3(x, y, z), radius));
            }
        }
        protected override void LoadContent()
        {
            ceffect = game.Content.Load<Effect>("effects");
            modelTarget = Loader.loadModel(game, "target");
            
            base.LoadContent();
        }

        public override void Initialize()
        {
            base.Initialize();
        }

        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            for (int i = 0; i < targets.Count; ++i)
            {
                Matrix world = Matrix.CreateScale(targets[i].Radius) * Matrix.CreateTranslation(targets[i].Center);
                Drawing.drawModel(modelTarget, world, game);
            }
        }

        public float? Collision(Ray ray)
        {
            float? result = null;
            for (int i = 0; i < targets.Count; ++i)
            {
                result = ray.Intersects(targets[i]);
                if (result != null)
                {
                    targets.RemoveAt(i);
                    return result;
                }
            }
            return result;
        }
    }
}
