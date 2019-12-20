using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace _3D
{
    public class Camera: GameComponent
    {
        Game1 game;
        Actor target = null;
        Matrix view;
        Matrix projection;
        Vector3 fromVector = new Vector3(0f, 10f, -10f);

        public Camera(Game game)
            : base(game)
        {
            this.game = game as Game1;
            SetView();
            SetProjection();
        }

        public void SetView()
        {
            if (target != null)
            {
                view = Matrix.CreateLookAt(fromVector + target.Position, target.Position, Vector3.Up);
            }
            else
            {
                view = Matrix.CreateLookAt(new Vector3(0f, 10f, -10f), Vector3.Zero, Vector3.Up);
            }
        }
        public void SetRotation()
        {
            fromVector = Vector3.Transform(new Vector3(0, 10f, -10f), Matrix.CreateRotationY(target.Rotation));
        }

        private void SetProjection()
        {
            projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(45f), game.AspectRatio, 1f, 10000f);
        }

        public Matrix GetView()
        {
            return view;
        }

        public Matrix GetProjection()
        {
            return projection;
        }

        public void SetTarget(Actor target)
        {
            this.target = target;
        }

        public override void Update(GameTime gameTime)
        {
            if (target != null)
            {
                if (Input.State.IsKeyDown(Microsoft.Xna.Framework.Input.Keys.RightShift))
                {
                    SetRotation();
                }
                SetView();
            }
        }
    }
}
