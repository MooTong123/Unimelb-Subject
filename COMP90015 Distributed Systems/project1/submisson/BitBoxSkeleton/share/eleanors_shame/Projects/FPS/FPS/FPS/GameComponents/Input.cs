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


namespace FPS.GameComponents
{
    public class Input : Microsoft.Xna.Framework.GameComponent
    {
        #region Fields
        static KeyboardState state;
        static KeyboardState lastState;
        static MouseState mouse;
        static MouseState mouseStart;
        static MouseState lastMouse;

        private FirstPerson game;
        #endregion

        #region Constructors
        public Input(Game game)
            : base(game)
        {
            this.game = game as FirstPerson;
            Mouse.SetPosition(game.GraphicsDevice.Viewport.Width / 2, game.GraphicsDevice.Viewport.Height / 2);
            mouseStart = Mouse.GetState();
            mouse = mouseStart;
        }
        #endregion

        #region Game Component Methods
        public override void Initialize()
        {
            base.Initialize();
        }

        public override void Update(GameTime gameTime)
        {
            lastState = state;
            lastMouse = mouse;
            state = Keyboard.GetState();
            mouse = Mouse.GetState();

            scroll = lastMouse.ScrollWheelValue - mouse.ScrollWheelValue;

            if (!state.IsKeyDown(Keys.U))
            {
                Mouse.SetPosition(game.GraphicsDevice.Viewport.Width / 2, game.GraphicsDevice.Viewport.Height / 2);
            }

            base.Update(gameTime);
        }
        #endregion

        #region Keyboard
        public static KeyboardState State
        {
            get { return state; }
        }
        public static KeyboardState LastState
        {
            get { return lastState; }
        }
        
        public static bool keyUp(Keys key)
        {
            return state.IsKeyUp(key) && lastState.IsKeyDown(key);
        }
        public static bool keyDown(Keys key)
        {
            return state.IsKeyDown(key) && lastState.IsKeyUp(key);
        }
        #endregion
        #region Mouse
        public static MouseState MouseState
        {
            get { return mouse; }
        }
        public static MouseState MouseStart
        {
            get { return mouseStart; }
        }
        public static MouseState LastMouse
        {
            get { return lastMouse; }
        }
        public static int Scroll
        {
            get { return scroll; }
        }
        private static int scroll;
        #endregion
    }
}
