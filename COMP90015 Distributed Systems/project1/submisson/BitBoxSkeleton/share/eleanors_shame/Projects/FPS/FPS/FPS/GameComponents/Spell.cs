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

using FPS.Entities;

namespace FPS.GameComponents
{
    public class Spell : Microsoft.Xna.Framework.DrawableGameComponent
    {
        Collidable target;
        FirstPerson game;

        bool alive = true;

        Vector3 position;

        readonly Vector3 size = new Vector3(Global.TileSize / 4);

        VertexPositionColor[] vertices;

        public Spell(Game game, Collidable target, Vector3 position)
            : base(game)
        {
            this.game = game as FirstPerson;
            this.target = target;
            this.position = position;

            game.Components.Add(this);
        }

        public override void Initialize()
        {
            List<VertexPositionColor> vert = new List<VertexPositionColor>();
            Drawing.Cube(ref vert, new Vector3(0), size);
            vertices = vert.ToArray();

            base.Initialize();
        }

        public override void Update(GameTime gameTime)
        {
            if (!alive) return;
            if (game.level.blockCollision(new BoundingBox(position, position + size / 6)) != null)
            {
                alive = false;
                game.Components.Remove(this);
            }
            if (game.player.Box.Intersects(new BoundingBox(position, position + size)))
            {
                if (game.player.State != Player.States.block)
                {
                    game.player.hurt(5f, Damage.Nature);
                }
                alive = false;
                game.Components.Remove(this);
            }

            Vector3 velocity = target.Box.Min - position;
            velocity.Normalize();
            position += velocity * 20;
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            if (!alive) return;
            game.effect.CurrentTechnique = game.effect.Techniques["ColoredNoShading"];
            game.effect.Parameters["xWorld"].SetValue(Matrix.CreateTranslation(position));
            game.effect.Parameters["xView"].SetValue(game.player.view);
            game.effect.Parameters["xProjection"].SetValue(game.player.projection);
            game.effect.Parameters["xEnableLighting"].SetValue(true);
            game.effect.Parameters["xAmbient"].SetValue(0.5f);
            Vector3 lightDir = new Vector3(3, -2, 5);
            lightDir.Normalize();
            game.effect.Parameters["xLightDirection"].SetValue(lightDir);

            foreach (EffectPass pass in game.effect.CurrentTechnique.Passes)
            {
                pass.Apply();
                game.GraphicsDevice.DrawUserPrimitives(PrimitiveType.TriangleList, vertices, 0, vertices.Count() / 3);
            }
        }
    }
}
