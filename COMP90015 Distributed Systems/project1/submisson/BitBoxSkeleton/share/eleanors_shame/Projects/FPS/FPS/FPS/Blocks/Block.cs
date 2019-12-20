using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using FPS.GameComponents;

namespace FPS.Blocks
{
    public abstract class Block: Collidable
    {
        protected VertexPositionNormalTexture[] vertices;
        protected Vector3 size;
        
        protected Vector3 position;

        protected Block(Game game, Vector3 Position, string textureName)
            : base(game)
        {
            this.game = game as FirstPerson;
            position = Position + size;
            size = new Vector3(Global.TileSize);
            bb.Min = position - size;
            bb.Max = position;

            game.Components.Add(this);
            loadVertices();

            texture = game.Content.Load<Texture2D>(textureName);            
        }

        private void loadVertices()
        {
            List<VertexPositionNormalTexture> vert = new List<VertexPositionNormalTexture>();
            Drawing.Cube(ref vert, position - size, position);
            vertices = vert.ToArray();
        }

        public override void Update(GameTime gameTime)
        {
            base.update();
            base.Update(gameTime);
        }
        public override void Draw(GameTime gameTime)
        {
            game.player.Effects(texture);

            foreach (EffectPass pass in game.effect.CurrentTechnique.Passes)
            {
                pass.Apply();
                game.GraphicsDevice.DrawUserPrimitives(PrimitiveType.TriangleList, vertices, 0, vertices.Count() / 3);
            }
        }
        public bool Collision(BoundingBox other)
        {
            return bb.Intersects(other);
        }
    }
}
