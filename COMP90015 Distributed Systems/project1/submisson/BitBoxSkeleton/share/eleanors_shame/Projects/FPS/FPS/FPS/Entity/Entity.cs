using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using FPS.GameComponents;

namespace FPS.Entity
{
    public abstract class Entity: Collidable
    {
        protected int health;
        protected bool alive = true;

        protected FirstPerson game;
        protected Vector3 position;

        protected Texture2D texture;

        protected int id;

        private VertexPositionNormalTexture[] vertices;

        public Entity(Game game, int id, Vector3 position, string textureName, int maxHealth)
            : base(game)
        {
            game.Components.Add(this);

            this.game = game as FirstPerson;
            this.position = position;
            this.texture = game.Content.Load<Texture2D>(textureName);
            this.id = id;
            health = maxHealth;

            bb.Max = position;
            bb.Min = position - new Vector3(Global.tileSize);

            loadVertices();
        }

        private void loadVertices()
        {
            List<VertexPositionNormalTexture> vert = new List<VertexPositionNormalTexture>();
            Drawing.Cube(ref vert, position - new Vector3(Global.tileSize), position);
            vertices = vert.ToArray();
        }

        public override void Draw(GameTime gameTime)
        {
            if (!alive)
            {
                return;
            }
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

        /// <summary>
        /// hurts the entity
        /// </summary>
        /// <returns>true if the entity is dead, else false</returns>
        public virtual bool hurt(int amount = 1)
        {
            health -= amount;
            if (health <= 0)
            {
                alive = false;
                return true;
            }
            return false;
        }
    }
}
