using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace _3D
{
    public class ModelContainer
    {
        Model model;
        Game1 game;

        public ModelContainer(Game game, string filename)
        {
            this.game = game as Game1;
            model = game.Content.Load<Model>(filename);
        }

        public void Draw(Vector3 Position, Matrix Transforms)
        {
            Matrix[] transforms = new Matrix[model.Bones.Count];
            model.CopyAbsoluteBoneTransformsTo(transforms);

            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (BasicEffect effect in mesh.Effects)
                {
                    effect.EnableDefaultLighting();
                    effect.World = transforms[mesh.ParentBone.Index] * Transforms * Matrix.CreateTranslation(Position);
                    effect.View = game.Camera.GetView();
                    effect.Projection = game.Camera.GetProjection();
                }
                mesh.Draw();
            }
        }
    }
}
