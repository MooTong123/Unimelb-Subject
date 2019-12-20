using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace FPS.Items
{
    using InventoryItems;

    public class Item: DrawableGameComponent
    {
        protected FirstPerson game;
        protected Texture2D sprite;

        protected Vector3 velocity = new Vector3(0);
        protected Vector3 position;
        protected BoundingBox box;

        private Vector3 posPrevious;
        private const float friction = 1.05f;
        private InventoryItem item;

        bool falling = false;

        float rot = 0;

        BasicEffect basicEffect;

        protected readonly Vector3 size = new Vector3(Global.TileSize / 4);

        private VertexPositionTexture[] vertices = new VertexPositionTexture[6];
        public Item(Game game, InventoryItem item, Texture2D sprite, Vector3 position)
            : base(game)
        {
            this.game = game as FirstPerson;
            this.sprite = sprite;
            this.position = position;
            this.item = item;

            game.Components.Add(this);

            velocity = Global.randomVector();

            box = new BoundingBox(position - size, position);

            basicEffect = new BasicEffect(game.GraphicsDevice)
            {
                TextureEnabled = true,
                VertexColorEnabled = true,
            };

        }

        public override void Update(GameTime gameTime)
        {
            velocity.X /= friction;
            velocity.Z /= friction;
            if (falling)
            {
                velocity.Y -= FPS.GameComponents.Player.gravity;
            }

            box.Min = position - size * 4;
            box.Max = position + size * 4;

            if (box.Intersects(game.player.Box))
            {
                game.Components.Remove(this);
                game.ui.addItem(item);
            }

            box.Min = position - size;
            box.Max = position;

            BoundingBox under = box;
            under.Min.Y -= 1;
            under.Max.Y -= 1;

            BoundingBox? result = game.level.blockCollision(box);
            if (result == null)
            {
                falling = true;
            }

            if (velocity.X != 0)
            {
                posPrevious = position;
                position.X += velocity.X;
                box.Min = position - size;
                box.Max = position;
                result = game.level.blockCollision(box);
                if (result != null)
                {
                    position = posPrevious;
                }
            }

            if (velocity.Y != 0)
            {
                posPrevious = position;
                position.Y += velocity.Y;
                box.Min = position - size;
                box.Max = position;
                result = game.level.blockCollision(box);
                if (result != null)
                {
                    position = posPrevious;

                    falling = false;
                    velocity.Y = 0;
                }
            }
            if (velocity.Z != 0)
            {
                posPrevious = position;
                position.Z += velocity.Z;
                box.Min = position - size;
                box.Max = position;
                result = game.level.blockCollision(box);
                if (result != null)
                {
                    position = posPrevious;
                }
            }

            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            rot += 3;

            basicEffect.World = Matrix.CreateTranslation(new Vector3(-24, -24, 0))
                                * Matrix.CreateScale(0.2f, -0.2f, 0.2f)
                                * Matrix.CreateRotationY(MathHelper.ToRadians(rot))
                                * Matrix.CreateTranslation(position);
            basicEffect.View = game.player.view;
            basicEffect.Projection = game.player.projection;

            game.spriteBatch.Begin(0, null, null, DepthStencilState.DepthRead, RasterizerState.CullNone, basicEffect);
            game.spriteBatch.Draw(sprite,new Rectangle(0, 0, 48, 48), Color.White);
            game.spriteBatch.End();
            game.GraphicsDevice.BlendState = BlendState.Opaque;
            game.GraphicsDevice.DepthStencilState = DepthStencilState.Default;
        }
    }
}
