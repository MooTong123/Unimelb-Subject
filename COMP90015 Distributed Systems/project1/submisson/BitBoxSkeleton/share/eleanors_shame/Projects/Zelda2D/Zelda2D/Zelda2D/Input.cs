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


namespace Zelda2D
{
    public class Input : GameComponent
    {
        #region Fields
        static KeyboardState state;
        static KeyboardState lastState;
        static MouseState mouse;
        static MouseState mouseStart;
        static MouseState lastMouse;

        private Game1 game;
        #endregion

        #region Constructors
        public Input(Game game)
            : base(game)
        {
            this.game = game as Game1;
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

            if (keyDown(Keys.Escape))
            {
                game.Exit();
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

        public static bool leftDown()
        {
            return MouseState.LeftButton == ButtonState.Pressed && LastMouse.LeftButton == ButtonState.Released;
        }
        public static bool leftUp()
        {
            return MouseState.LeftButton == ButtonState.Released && LastMouse.LeftButton == ButtonState.Pressed;
        }

        public static bool rightDown()
        {
            return MouseState.RightButton == ButtonState.Pressed && LastMouse.RightButton == ButtonState.Released;
        }
        public static bool rightUp()
        {
            return MouseState.RightButton == ButtonState.Released && LastMouse.RightButton == ButtonState.Pressed;
        }

        public static int Scroll
        {
            get { return scroll; }
        }
        private static int scroll;
        #endregion
    }
}
