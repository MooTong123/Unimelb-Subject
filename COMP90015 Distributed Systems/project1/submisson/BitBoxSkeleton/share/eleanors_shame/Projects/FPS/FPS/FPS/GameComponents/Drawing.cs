using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.GameComponents
{
    public static class Drawing
    {
        private static int t;

        public static void Cube(ref List<VertexPositionNormalTexture> vertices, Vector3 position1, Vector3 position2)
        {
            float size = Math.Abs(position1.X - position2.X);
            Vector3 position = XMath.Mean(position1, position2);

            Vector3 btmLeftFront = position + new Vector3(-0.5f, -0.5f, 0.5f) * size;
            Vector3 btmLeftBack = position + new Vector3(-0.5f, 0.5f, 0.5f) * size;
            Vector3 btmRightFront = position + new Vector3(0.5f, -0.5f, 0.5f) * size;
            Vector3 btmRightBack = position + new Vector3(0.5f, 0.5f, 0.5f) * size;

            Vector3 topLeftFront = position + new Vector3(-0.5f, -0.5f, -0.5f) * size;
            Vector3 topLeftBack = position + new Vector3(-0.5f, 0.5f, -0.5f) * size;
            Vector3 topRightFront = position + new Vector3(0.5f, -0.5f, -0.5f) * size;
            Vector3 topRightBack = position + new Vector3(0.5f, 0.5f, -0.5f) * size;

            Vector2 texTopLeft = new Vector2(1, 0);
            Vector2 texTopRight = new Vector2(0, 0);
            Vector2 texBottomLeft = new Vector2(1, 1);
            Vector2 texBottomRight = new Vector2(0, 1);

            Vector3 normalFront = new Vector3(0.0f, 0.0f, 1.0f) * size;
            Vector3 normalBack = new Vector3(0.0f, 0.0f, -1.0f) * size;
            Vector3 normalTop = new Vector3(0.0f, 1.0f, 0.0f) * size;
            Vector3 normalBottom = new Vector3(0.0f, -1.0f, 0.0f) * size;
            Vector3 normalLeft = new Vector3(-1.0f, 0.0f, 0.0f) * size;
            Vector3 normalRight = new Vector3(1.0f, 0.0f, 0.0f) * size;

            // front
            vertices.Add(new VertexPositionNormalTexture(topLeftFront, normalFront, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmLeftFront, normalFront, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(topRightFront, normalFront, texTopRight));
            vertices.Add(new VertexPositionNormalTexture(btmLeftFront, normalFront, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightFront, normalFront, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(topRightFront, normalFront, texTopRight));

            // back
            vertices.Add(new VertexPositionNormalTexture(topLeftBack, normalBack, texTopRight));
            vertices.Add(new VertexPositionNormalTexture(topRightBack, normalBack, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmLeftBack, normalBack, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(btmLeftBack, normalBack, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(topRightBack, normalBack, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightBack, normalBack, texBottomLeft));

            // top
            vertices.Add(new VertexPositionNormalTexture(topLeftFront, normalTop, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(topRightBack, normalTop, texTopRight));
            vertices.Add(new VertexPositionNormalTexture(topLeftBack, normalTop, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(topLeftFront, normalTop, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(topRightFront, normalTop, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(topRightBack, normalTop, texTopRight));

            // bottom
            vertices.Add(new VertexPositionNormalTexture(btmLeftFront, normalBottom, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmLeftBack, normalBottom, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightBack, normalBottom, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(btmLeftFront, normalBottom, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightBack, normalBottom, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(btmRightFront, normalBottom, texTopRight));

            // left
            vertices.Add(new VertexPositionNormalTexture(topLeftFront, normalLeft, texTopRight));
            vertices.Add(new VertexPositionNormalTexture(btmLeftBack, normalLeft, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(btmLeftFront, normalLeft, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(topLeftBack, normalLeft, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmLeftBack, normalLeft, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(topLeftFront, normalLeft, texTopRight));

            // right
            vertices.Add(new VertexPositionNormalTexture(topRightFront, normalRight, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightFront, normalRight, texBottomLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightBack, normalRight, texBottomRight));
            vertices.Add(new VertexPositionNormalTexture(topRightBack, normalRight, texTopRight));
            vertices.Add(new VertexPositionNormalTexture(topRightFront, normalRight, texTopLeft));
            vertices.Add(new VertexPositionNormalTexture(btmRightBack, normalRight, texBottomRight));
        }
        public static void Cube(ref List<VertexPositionColor> vertices, Vector3 position1, Vector3 position2)
        {
            float size = Math.Abs(position1.X - position2.X);
            Vector3 position = XMath.Mean(position1, position2);

            Vector3 btmLeftFront = position + new Vector3(-0.5f, -0.5f, 0.5f) * size;
            Vector3 btmLeftBack = position + new Vector3(-0.5f, 0.5f, 0.5f) * size;
            Vector3 btmRightFront = position + new Vector3(0.5f, -0.5f, 0.5f) * size;
            Vector3 btmRightBack = position + new Vector3(0.5f, 0.5f, 0.5f) * size;

            Vector3 topLeftFront = position + new Vector3(-0.5f, -0.5f, -0.5f) * size;
            Vector3 topLeftBack = position + new Vector3(-0.5f, 0.5f, -0.5f) * size;
            Vector3 topRightFront = position + new Vector3(0.5f, -0.5f, -0.5f) * size;
            Vector3 topRightBack = position + new Vector3(0.5f, 0.5f, -0.5f) * size;

            Vector2 texTopLeft = new Vector2(1, 0);
            Vector2 texTopRight = new Vector2(0, 0);
            Vector2 texBottomLeft = new Vector2(1, 1);
            Vector2 texBottomRight = new Vector2(0, 1);

            Vector3 normalFront = new Vector3(0.0f, 0.0f, 1.0f) * size;
            Vector3 normalBack = new Vector3(0.0f, 0.0f, -1.0f) * size;
            Vector3 normalTop = new Vector3(0.0f, 1.0f, 0.0f) * size;
            Vector3 normalBottom = new Vector3(0.0f, -1.0f, 0.0f) * size;
            Vector3 normalLeft = new Vector3(-1.0f, 0.0f, 0.0f) * size;
            Vector3 normalRight = new Vector3(1.0f, 0.0f, 0.0f) * size;

            // front
            vertices.Add(new VertexPositionColor(topLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(topRightFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightFront, Color.Black));
            vertices.Add(new VertexPositionColor(topRightFront, Color.Black));

            // back
            vertices.Add(new VertexPositionColor(topLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(topRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(topRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightBack, Color.Black));

            // top
            vertices.Add(new VertexPositionColor(topLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(topRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(topLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(topLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(topRightFront, Color.Black));
            vertices.Add(new VertexPositionColor(topRightBack, Color.Black));

            // bottom
            vertices.Add(new VertexPositionColor(btmLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightFront, Color.Black));

            // left
            vertices.Add(new VertexPositionColor(topLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftFront, Color.Black));
            vertices.Add(new VertexPositionColor(topLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(btmLeftBack, Color.Black));
            vertices.Add(new VertexPositionColor(topLeftFront, Color.Black));

            // right
            vertices.Add(new VertexPositionColor(topRightFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(topRightBack, Color.Black));
            vertices.Add(new VertexPositionColor(topRightFront, Color.Black));
            vertices.Add(new VertexPositionColor(btmRightBack, Color.Black));
        }
        public static void drawModel(Model model, Matrix world, FirstPerson game)
        {
            Matrix[] transforms = new Matrix[model.Bones.Count];
            model.CopyAbsoluteBoneTransformsTo(transforms);

            foreach (ModelMesh mesh in model.Meshes) {
                foreach (BasicEffect effect in mesh.Effects) {
                    Matrix nWorld = world;
                    effect.World = nWorld;
                    effect.View = game.player.view;
                    effect.Projection = game.player.projection;
                }
                mesh.Draw();
            }
            t++;
        }
    }
}
