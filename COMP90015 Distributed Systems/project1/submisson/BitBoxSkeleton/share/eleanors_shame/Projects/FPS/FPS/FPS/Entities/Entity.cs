using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using FPS.GameComponents;

namespace FPS.Entities
{
    public abstract class Entity: Unit
    {
        protected float health;
        public override float Health
        {
            get
            {
                return health;
            }
        }
        protected bool alive = true;

        protected Vector3 position;

        private VertexPositionNormalTexture[] vertices;

        public Entity(Game game,Vector3 position, string textureName)
            : base(game)
        {
            game.Components.Add(this);

            this.game = game as FirstPerson;
            this.position = position;
            this.texture = game.Content.Load<Texture2D>(textureName);

            bb.Max = position;
            bb.Min = position - new Vector3(Global.TileSize);

            loadVertices();
        }

        private void loadVertices()
        {
            List<VertexPositionNormalTexture> vert = new List<VertexPositionNormalTexture>();
            Drawing.Cube(ref vert, position - new Vector3(Global.TileSize), position);
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
        public override bool hurt(float amount, Damage type)
        {
            amount *= 1f - resist[type];
            if (onHurt(amount, type))
            {
                health -= amount;
                if (health <= 0)
                {
                    alive = false;
                    game.Components.Remove(this);
                    drop();
                    return true;
                }
            }
            return false;
        }

        public virtual bool onHurt(float amount, Damage type)
        {
            return true;
        }

        public abstract void drop();
    }
}
