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

using FPS.GameComponents;

namespace FPS
{
    public enum Damage
    {
        Physical,
        Fire,
        Frost,
        Nature,
        Arcane,
        Holy,
        Shadow,
        True
    }
    public class FirstPerson : Microsoft.Xna.Framework.Game
    {
        GameTime lastTime;
        public GameTime Time
        {
            get { return lastTime; }
        }
        GraphicsDeviceManager graphics;

        public SpriteBatch spriteBatch;
        public Effect effect;
        public Player player;
        public Input input;
        public Level level;
        public TargetManager enemies;
        public UI ui;

        public SpriteFont font;

        public FirstPerson()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";            
        }

        protected override void Initialize()
        {
            
            Components.Add(level = new Level(this));
            Components.Add(player = new Player(this));
            Components.Add(input = new Input(this));
            
            
            //Components.Add(enemies = new TargetManager(this));
            effect = Content.Load<Effect>("effects");

            base.Initialize();
        }

        protected override void LoadContent()
        {
            Components.Add(ui = new UI(this));
            spriteBatch = new SpriteBatch(GraphicsDevice);
            font = Content.Load<SpriteFont>("font");
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            lastTime = gameTime;

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(new Color(217, 62, 255));

            base.Draw(gameTime);
        }
    }
}
