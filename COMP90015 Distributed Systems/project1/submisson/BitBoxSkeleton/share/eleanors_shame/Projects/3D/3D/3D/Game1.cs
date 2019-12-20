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

namespace _3D
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Input input;
        Camera camera;
        float aspectRatio;
        ActorManager actorManager;
        Player player;

        ModelContainer plane;

        public Input Input { get { return input; } }
        public GraphicsDevice device { get { return graphics.GraphicsDevice; } }
        public Camera Camera { get { return camera; } }
        public float AspectRatio { get { return aspectRatio; } }
        public ActorManager ActorManager { get { return actorManager; } }
        
        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        protected override void Initialize()
        {
            aspectRatio = device.Viewport.AspectRatio;

            Components.Add(camera = new Camera(this));
            Components.Add(actorManager = new ActorManager(this));
            Components.Add(input = new Input(this));
            Components.Add(player = new Player(this));
                        
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            plane = new ModelContainer(this, "Models/plane");

            base.LoadContent();
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (Input.keyDown(Keys.Escape))
            {
                Exit();
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            plane.Draw(new Vector3(0, -1.25f, 0), Matrix.Identity);
            base.Draw(gameTime);
        }
    }
}
