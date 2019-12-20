using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

using FPS.Items.InventoryItems;

// TODO: separate into Inventory class

namespace FPS.GameComponents
{
    public class UI: DrawableGameComponent
    {
        FirstPerson game;

        Texture2D inventory;
        Texture2D selection;

        Texture2D primitive;

        public SpriteFont font;
        SpriteFont numberFont;

        int select = 0;
        const int maxSelect = 4;
        InventoryItem[] items = new InventoryItem[maxSelect + 1];

        public InventoryItem Item
        {
            get { return items[select]; }
        }

        public bool addItem(InventoryItem item, int amount = 1)
        {
            int i;
            for (i = 0; i < items.Count(); ++i)
            {
                if (items[i] == null)
                {
                    break;
                }
                if (items[i].Id == item.Id)
                {
                    items[i].count += amount;;
                    return true;
                }
            }
            if (i == items.Count())
            {
                return false;
            }

            items[i] = item;
            items[i].count = amount;
            return true;
        }
        public bool removeItem(InventoryItem item, int amount = 0)
        {
            for (int i = 0; i < items.Count(); ++i)
            {
                if (items[i] != null)
                {
                    if (items[i].Id == item.Id)
                    {
                        if (amount == 0)
                        {
                            items[i] = null;
                        }
                        else
                        {
                            items[i].count -= amount;
                        }
                        return true;
                    }
                }
            }
            return false;
        }



        public UI(Game game)
            : base(game)
        {
            this.game = game as FirstPerson;

            inventory = game.Content.Load<Texture2D>("inventory");
            selection = game.Content.Load<Texture2D>("select");
            primitive = game.Content.Load<Texture2D>("primitive");

            numberFont = game.Content.Load<SpriteFont>("numbers");
            font = game.Content.Load<SpriteFont>("font");

            items[0] = new Sword(game);
            items[0].count = 1;
        }

        public override void Update(GameTime gameTime)
        {
            if (Input.Scroll < 0)
            {
                select++;
                if (select > maxSelect)
                {
                    select = 0;
                }
            }
            else if (Input.Scroll > 0)
            {
                select--;
                if (select < 0)
                {
                    select = maxSelect;
                }
            }

            if (Input.keyDown(Keys.D1)) select = 0;
            if (Input.keyDown(Keys.D2)) select = 1;
            if (Input.keyDown(Keys.D3)) select = 2;
            if (Input.keyDown(Keys.D4)) select = 3;
            if (Input.keyDown(Keys.D5)) select = 4;
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            game.spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.AlphaBlend);

            // inventory bar
            game.spriteBatch.Draw(inventory, new Rectangle(282, 430, 236, 48), Color.White);
            for (int i = 0; i <= maxSelect; ++i)
            {
                game.spriteBatch.DrawString(numberFont, (i + 1).ToString(), new Vector2(282 + i * 48, 430), Color.White);
                if (items[i] != null)
                {
                    if (items[i].count == 0)
                    {
                        items[i] = null;
                    }
                    else
                    {
                        items[i].draw(i);
                        if (items[i].count > 1) {
                            game.spriteBatch.DrawString(numberFont, items[i].count.ToString(), new Vector2(318 + i * 48, 458), Color.Black);
                        }
                    }
                }
            }
            game.spriteBatch.Draw(selection, new Rectangle(282 + select * 47, 430, 48, 48), Color.White);

            DrawUnitFrame(game.player, new Vector2(32, 414), new Vector2(128, 32));
            if (game.player.Target != null)
            {
                DrawUnitFrameRight(game.player.Target, new Vector2(800-32, 414), new Vector2(128, 32));
            }

            game.spriteBatch.End();
            game.GraphicsDevice.BlendState = BlendState.Opaque;
            game.GraphicsDevice.DepthStencilState = DepthStencilState.Default;

            base.Draw(gameTime);
        }

        private void DrawUnitFrame(Unit unit, Vector2 position, Vector2 size)
        {
            // background
            game.spriteBatch.Draw(primitive, new Rectangle((int)position.X, (int)position.Y, (int)size.X, (int)size.Y), Color.Black);
            game.spriteBatch.Draw(primitive,
                new Rectangle((int)(position.X), (int)(position.Y + size.Y / 2), (int)size.X, (int)(size.Y / 2)),
                Color.DarkRed);

            // health bar
            int end = (int)size.X;

            end = (int)((float)end * (unit.Health / unit.MaxHealth));
            game.spriteBatch.Draw(primitive,
                                  new Rectangle((int)position.X, (int)(position.Y + size.Y / 2), end, (int)(size.Y / 2)),
                                  Color.Lime);

            // name
            game.spriteBatch.DrawString(font, unit.Name, position, Color.White);
            game.spriteBatch.DrawString(numberFont, ((int)unit.Health).ToString() + "/" + unit.MaxHealth.ToString(),
                                        new Vector2(position.X, position.Y + size.Y / 2), Color.Black);
        }
        private void DrawUnitFrameRight(Unit unit, Vector2 position, Vector2 size)
        {
            // background
            game.spriteBatch.Draw(primitive,
                new Rectangle((int)(position.X - size.X), (int)position.Y, (int)size.X, (int)size.Y / 2), Color.Black);
            game.spriteBatch.Draw(primitive,
                new Rectangle((int)(position.X - size.X), (int)(position.Y + size.Y / 2), (int)size.X, (int)(size.Y / 2)),
                Color.DarkRed);

            // health bar
            int end = (int)(size.X);

            end = (int)((float)end * (unit.Health / unit.MaxHealth));
            game.spriteBatch.Draw(primitive,
                                  new Rectangle((int)(position.X - end), (int)(position.Y + size.Y / 2),
                                                (int)end, (int)size.Y / 2),
                                  Color.Lime);

            // name
            game.spriteBatch.DrawString(font, unit.Name, position, Color.White, 0, new Vector2(font.MeasureString(unit.Name).X, 0),
                                        1, new SpriteEffects(), 0);
            game.spriteBatch.DrawString(numberFont, ((int)unit.Health).ToString() + "/" + unit.MaxHealth.ToString(),
                                        new Vector2(
                                            position.X - 
                                            font.MeasureString(unit.Health.ToString() + "/" + unit.MaxHealth.ToString()).X,
                                            position.Y + size.Y / 2), Color.Black);
        }
    }
}
