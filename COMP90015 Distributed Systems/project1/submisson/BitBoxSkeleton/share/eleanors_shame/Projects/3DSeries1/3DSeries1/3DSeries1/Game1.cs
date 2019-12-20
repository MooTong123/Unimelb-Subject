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

namespace _3DSeries1
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        SpriteFont font;
        Texture2D heightmap;

        GraphicsDevice device;
        Effect effect;

        Matrix view;
        Matrix projection;

        VertexPositionColor[] vertices;
        int[] indices;

        int tWidth = 4;
        int tHeight = 3;
        float[,] heightData;

        float angle = 0;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }
        protected override void Initialize()
        {
            base.Initialize();
        }
        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            font = Content.Load<SpriteFont>("font");
            effect = Content.Load<Effect>("effects");

            heightmap = Content.Load<Texture2D>("heightmap");

            device = graphics.GraphicsDevice;
            graphics.PreferredBackBufferWidth = 500;
            graphics.PreferredBackBufferHeight = 500;
            graphics.IsFullScreen = false;
            graphics.ApplyChanges();

            Window.Title = "3D tutorial 1";

            setupCamera();

            loadHeightmap(heightmap);
            setupVertices();
            setupIndices();
        }
        protected override void UnloadContent()
        {
        }
        protected override void Update(GameTime gameTime)
        {
            KeyboardState st = Keyboard.GetState();
            if (st.IsKeyDown(Keys.Left))
            {
                angle += 0.05f;
            }
            if (st.IsKeyDown(Keys.Right))
            {
                angle -= 0.05f;
            }
            base.Update(gameTime);
        }
        protected override void Draw(GameTime gameTime)
        {
            device.Clear(ClearOptions.Target | ClearOptions.DepthBuffer, Color.Black, 1.0f, 0);

            RasterizerState rs = new RasterizerState();
            rs.CullMode = CullMode.None;
            //rs.FillMode = FillMode.WireFrame;
            device.RasterizerState = rs;

            effect.CurrentTechnique = effect.Techniques["ColoredNoShading"];
            effect.Parameters["xView"].SetValue(view);
            effect.Parameters["xProjection"].SetValue(projection);
            effect.Parameters["xWorld"].SetValue(Matrix.CreateTranslation(-tWidth / 2.0f, 0, tHeight / 2.0f) * Matrix.CreateRotationY(angle));
            foreach (EffectPass p in effect.CurrentTechnique.Passes)
            {
                p.Apply();
            }
            device.DrawUserIndexedPrimitives(PrimitiveType.TriangleList, vertices, 0, vertices.Length, indices, 0,
                                      indices.Length / 3, VertexPositionColor.VertexDeclaration);

            spriteBatch.Begin();
            spriteBatch.DrawString(font, "FPS: " + Utility.framerate().ToString(), new Vector2(10, 10), Color.Black);
            spriteBatch.End();

            base.Draw(gameTime);
        }

        void setupVertices()
        {
            float minHeight = float.MaxValue;
            float maxHeight = float.MinValue;
            for (int x = 0; x < tHeight; x++)
            {
                for (int y = 0; y < tHeight; y++)
                {
                    if (heightData[x, y] < minHeight)
                        minHeight = heightData[x, y];
                    if (heightData[x, y] > maxHeight)
                        maxHeight = heightData[x, y];
                }
            }

            vertices = new VertexPositionColor[tWidth * tHeight];

            for (int x = 0; x < tWidth; ++x)
            {
                for (int y = 0; y < tHeight; ++y)
                {
                    vertices[x + y * tWidth].Position = new Vector3(x, heightData[x, y], -y);
                    if (heightData[x, y] < minHeight + (maxHeight - minHeight) / 4)
                    {
                        vertices[x + y * tWidth].Color = Color.Blue;
                    }
                    else if (heightData[x, y] < minHeight + (maxHeight - minHeight) * 2 / 4)
                    {
                        vertices[x + y * tWidth].Color = Color.Green;
                    }
                    else if (heightData[x, y] < minHeight + (maxHeight - minHeight) * 3 / 4)
                    {
                        vertices[x + y * tWidth].Color = Color.Brown;
                    }
                    else
                    {
                        vertices[x + y * tWidth].Color = Color.White;
                    }
                }
            }
        }
        void setupCamera()
        {
            view = Matrix.CreateLookAt(new Vector3(60, 80, -80), new Vector3(0, 0, 0), new Vector3(0, 1, 0));
            projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver4, device.Viewport.AspectRatio, 1, 300);
        }
        void setupIndices()
        {
            indices = new int[(tWidth - 1) * (tHeight - 1) * 6];
            int c = 0;
            for (int x = 0; x < tWidth - 1; ++x)
            {
                for (int y = 0; y < tHeight - 1; ++y)
                {
                    int lowerleft = x + y * tWidth;
                    int lowerright = (x + 1) + y * tWidth;
                    int topleft = x + (y + 1) * tWidth;
                    int topright = (x + 1) + (y + 1) * tWidth;

                    indices[c++] = topleft; 
                    indices[c++] = lowerright;
                    indices[c++] = lowerleft;

                    indices[c++] = topleft;
                    indices[c++] = topright;
                    indices[c++] = lowerright;
                }
            }
        }

        void loadHeightmap(Texture2D map)
        {
            tWidth = map.Width;
            tHeight = map.Height;

            Color[] mapCols = new Color[tWidth * tHeight];
            map.GetData(mapCols);
            
            heightData = new float[tWidth, tHeight];
            for (int x = 0; x < tWidth; ++x)
            {
                for (int y = 0; y < tHeight; ++y)
                {
                    heightData[x, y] = mapCols[x + y * tWidth].R / 5.0f;
                }
            }
        }
    }
}
