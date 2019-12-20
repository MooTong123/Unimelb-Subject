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

namespace Physics
{
    public class Physics : Microsoft.Xna.Framework.Game
    {
        public static SpriteFont font;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        public SpriteBatch SpriteBatch()
        {
            return spriteBatch;
        }

        public Physics()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            Components.Add(new Input(this));

            // left wall
            for (int i = 0; i < 480; i += 32)
            {
                Components.Add(new Box(this, new Vector2(0, i), 100000, false, false));
            }
            // right wall
            for (int i = 0; i < 480; i += 32)
            {
                Components.Add(new Box(this, new Vector2(768, i), 100000, false, false));
            }
            // roof
            for (int i = 32; i < 768; i += 32)
            {
                Components.Add(new Box(this, new Vector2(i, 0), 100000, false, false));
            }
            // floor
            for (int i = 32; i < 768; i += 32)
            {
                Components.Add(new Box(this, new Vector2(i, 448), 100000, false, false));
            }

            Components.Add(new Box(this, new Vector2(100, 382), 1, true));
            Components.Add(new Box(this, new Vector2(200, 382), 1));
            Components.Add(new Box(this, new Vector2(300, 382), 1));
            Components.Add(new Box(this, new Vector2(400, 382), 1));
        }

        protected override void Initialize()
        {   
            base.Initialize();
        }
        
        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            font = Content.Load<SpriteFont>("font");
        }
        
        protected override void Update(GameTime gameTime)
        {
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
            spriteBatch.Begin();
            base.Draw(gameTime);
            spriteBatch.End();
        }
    }
}
