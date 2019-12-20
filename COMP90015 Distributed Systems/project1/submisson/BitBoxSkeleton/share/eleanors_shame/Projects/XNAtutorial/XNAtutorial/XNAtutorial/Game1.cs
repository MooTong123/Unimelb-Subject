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

namespace XNAtutorial
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        GraphicsDevice device;

        Texture2D texBack;
        Texture2D texFore;
        Texture2D texGround;
        Texture2D texCarriage;
        Texture2D texCannon;
        Texture2D texRocket;
        Texture2D texSmoke;
        Texture2D texExplosion;

        List<Vector2> smokeList = new List<Vector2>();
        Random rand = new Random();

        SpriteFont font;

        float pScale;

        int width;
        int height;

        #region structures
        class CollData
        {
            public CollData(bool c, Vector2 p) { collides = c; pos = p; }

            public bool collides;
            public Vector2 pos;

            public static implicit operator bool(CollData cd)
            {
                return cd.collides;
            }
        }
        struct CollObj
        {
            public CollObj(Color[,] t, Matrix m) { tex = t; mat = m; }

            public Color[,] tex;
            public Matrix mat;
        }
        struct Player
        {
            public Vector2 position;
            public bool isAlive;
            public Color color;
            public float angle;
            public float power;
        }
        struct Rocket
        {
            public bool flying;
            public Vector2 pos;
            public Vector2 dir;
            public float angle;
            public float scale;
        }
        struct Particle
        {
            public float tBirth;
            public float maxAge;
            public Vector2 start;
            public Vector2 acceleration;
            public Vector2 direction;
            public Vector2 position;
            public float scale;
            public Color col;
        }
        #endregion
        Rocket rocket;
        List<Particle> particles = new List<Particle>();

        Player[] players;
        const int num_players = 4;

        int cPlayer = 0;

        int[] contour;

        #region xna

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            rocket.flying = false;
            rocket.scale = 0.1f;
        }

        protected override void Initialize()
        {
            graphics.PreferredBackBufferWidth = 500;
            graphics.PreferredBackBufferHeight = 500;
            graphics.IsFullScreen = false;
            graphics.ApplyChanges();

            base.Initialize();
        }

        protected override void LoadContent()
        {
            device = graphics.GraphicsDevice;
            spriteBatch = new SpriteBatch(GraphicsDevice);

            width = device.PresentationParameters.BackBufferWidth;
            height = device.PresentationParameters.BackBufferHeight;

            texBack = Content.Load<Texture2D>("background");
            texCarriage = Content.Load<Texture2D>("carriage");
            texCannon = Content.Load<Texture2D>("cannon");
            texRocket = Content.Load<Texture2D>("rocket");
            texSmoke = Content.Load<Texture2D>("smoke");
            texGround = Content.Load<Texture2D>("ground");
            texExplosion = Content.Load<Texture2D>("explosion");

            genTerrain();
            setupPlayers();
            flatten();
            makeTexture();

            font = Content.Load<SpriteFont>("font");

            pScale = 40.0f / (float)texCarriage.Width;

            Window.Title = "Reimer's 2D XNA Tutorial";
        }
        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            processKeys();

            if (rocket.flying)
            {
                updateRocket();
                check(gameTime);
            }

            if (particles.Count() > 0)
            {
                updateParticles(gameTime);
            }

            base.Update(gameTime);
        }
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin();
            drawScene();
            drawPlayers();
            drawRocket();
            drawSmoke();
            spriteBatch.DrawString(font, "Cannon power: " + players[cPlayer].power.ToString(), new Vector2(20, 45), players[cPlayer].color);
            spriteBatch.DrawString(font, "Cannon angle: " + MathHelper.ToDegrees(players[cPlayer].angle).ToString(), new Vector2(20, 20), players[cPlayer].color);
            spriteBatch.End();

            spriteBatch.Begin(SpriteSortMode.Deferred, BlendState.Additive);
            drawExplosion();
            spriteBatch.End();

            base.Draw(gameTime);
        }

        #endregion
        #region core
        void processKeys()
        {
            KeyboardState st = Keyboard.GetState();
            if (st.IsKeyDown(Keys.Left)) {
                players[cPlayer].angle -= 0.01f;
            }
            if (st.IsKeyDown(Keys.Right)) {
                players[cPlayer].angle += 0.01f;
            }
            if (players[cPlayer].angle > MathHelper.PiOver2) {
                players[cPlayer].angle = -MathHelper.PiOver2;
            }
            if (players[cPlayer].angle < -MathHelper.PiOver2) {
                players[cPlayer].angle = MathHelper.PiOver2;
            }

            if (st.IsKeyDown(Keys.Down)) {
                players[cPlayer].power -= 1;
            }
            if (st.IsKeyDown(Keys.Up)) {
                players[cPlayer].power += 1;
            }

            if (players[cPlayer].power > 1000) {
                players[cPlayer].power = 1000;
            }
            if (players[cPlayer].power < 0) {
                players[cPlayer].power = 0;
            }

            if (!rocket.flying && (st.IsKeyDown(Keys.Enter) || st.IsKeyDown(Keys.Space))) {
                rocket.flying = true;
                rocket.pos = players[cPlayer].position;
                rocket.pos.X += 20;
                rocket.pos.Y -= 10;
                rocket.angle = players[cPlayer].angle;

                Vector2 up = new Vector2(0, -1);
                Matrix rot = Matrix.CreateRotationZ(rocket.angle);
                rocket.dir = Vector2.Transform(up, rot);
                rocket.dir *= players[cPlayer].power / 50.0f;
            }

            if (st.IsKeyDown(Keys.R)) {
                rocket.flying = false;
            }
        }

        void drawScene()
        {
            Rectangle screen = new Rectangle(0, 0, width, height);
            spriteBatch.Draw(texBack, screen, Color.White);
            spriteBatch.Draw(texFore, screen, Color.White);
        }
        void drawPlayers()
        {
            foreach (Player p in players) {
                if (p.isAlive) {
                    spriteBatch.Draw(texCarriage, p.position, null, p.color, 0, new Vector2(0, texCarriage.Height), pScale, SpriteEffects.None, 0);
                    
                    int xp = (int)p.position.X;
                    int yp = (int)p.position.Y;
                    Vector2 o = new Vector2(11, 50);

                    spriteBatch.Draw(texCannon, new Vector2(xp + 20, yp - 10), null, p.color, p.angle, o, pScale, SpriteEffects.None, 1);
                }
            }
        }
        void drawRocket()
        {
            if (rocket.flying) {
                spriteBatch.Draw(texRocket, rocket.pos, null, players[cPlayer].color, rocket.angle, new Vector2(42, 240), 0.1f, SpriteEffects.None, 1);
            }
        }
        void drawSmoke()
        {
            for (int i = 0; i < smokeList.ToArray().Length; ++i) {
                if (rand.Next(50) == 1) {
                    smokeList.RemoveAt(i);
                }
                else {
                    spriteBatch.Draw(texSmoke, smokeList[i], null, Color.White, 0, new Vector2(40, 35), 0.2f, SpriteEffects.None, 1);
                }
            }
        }
        void drawExplosion()
        {
            for (int i = 0; i < particles.Count(); ++i) {
                Particle p = particles[i];
                spriteBatch.Draw(texExplosion, p.position, null, p.col, i, new Vector2(256, 256), p.scale, SpriteEffects.None, 1);
            }
        }

        void explosion(Vector2 pos, int number, float size, float maxAge, GameTime t)
        {
            for (int i = 0; i < number; ++i)
            {
                addParticle(pos, size, maxAge, t);

                Matrix m = Matrix.CreateTranslation(-texExplosion.Width / 2, -texExplosion.Height / 2, 0) *
                           Matrix.CreateRotationZ((float)rand.Next(10)) * Matrix.CreateScale(size / (float)texExplosion.Width * 2.0f)
                           * Matrix.CreateTranslation(pos.X, pos.Y, 0);
                addCrater(m);
                for (int j = 0; j < players.Length; ++j)
                {
                    players[j].position.Y = contour[(int)players[j].position.X];
                }
                flatten();
                makeTexture();
            }
        }
        void addParticle(Vector2 pos, float size, float maxAge, GameTime t)
        {
            Particle p = new Particle();

            p.start = pos;
            p.position = pos;

            p.tBirth = (float)t.TotalGameTime.TotalMilliseconds;
            p.maxAge = maxAge;
            p.scale = 0.25f;
            p.col = Color.White;

            float dist = (float)rand.NextDouble() * size;
            Vector2 displacement = new Vector2(dist, 0);
            float angle = MathHelper.ToRadians(rand.Next(360));
            displacement = Vector2.Transform(displacement, Matrix.CreateRotationZ(angle));

            p.direction = 2.0f * displacement;
            p.acceleration = p.direction;

            particles.Add(p);
        }

        void addCrater(Matrix mat)
        {
            Color[,] tex = tex2array(texExplosion);

            int w = tex.GetLength(0);
            int h = tex.GetLength(1);

            for (int x = 0; x < w; ++x)
            {
                for (int y = 0; y < h; ++y)
                {
                    if (tex[x, y].R > 10)
                    {
                        Vector2 imgPos = new Vector2(x, y);
                        Vector2 screenPos = Vector2.Transform(imgPos, mat);

                        int sx = (int)screenPos.X;
                        int sy = (int)screenPos.Y;

                        if (sx > 0 && sx < width && contour[sx] < sy)
                        {
                            contour[sx] = sy;
                        }
                    }
                }
            }
        }

        void setupPlayers()
        {
            Color[] col = new Color[4];
            col[0] = Color.Red;
            col[1] = Color.Green;
            col[2] = Color.Blue;
            col[3] = Color.Yellow;

            players = new Player[num_players];

            for (int i = 0; i < 4; ++i) {
                players[i].isAlive = true;
                players[i].color = col[i];
                players[i].angle = MathHelper.ToRadians(90);
                players[i].power = 100;

                players[i].position = new Vector2();
                players[i].position.X = width / (num_players + 1) * (i + 1);
                players[i].position.Y = contour[(int)players[i].position.X];
            }
        }
        void flatten()
        {
            foreach (Player player in players) {
                if (player.isAlive) {
                    for (int x = 0; x < 40; x++) {
                        contour[(int)player.position.X + x] = contour[(int)player.position.X];
                    }
                }
            }
        }

        void genTerrain()
        {
            contour = new int[width];

            double r1 = rand.NextDouble() + 1;
            double r2 = rand.NextDouble() + 2;
            double r3 = rand.NextDouble() + 3;

            float offset = height / 2;
            float peakHeight = 100;
            float flatness = 50;

            for (int x = 0; x < width; ++x) {
                double h = peakHeight / r1 * Math.Sin((float)x / flatness * r1 + r1);
                h += peakHeight / r2 * Math.Sin((float)x / flatness * r2 + r2);
                h += peakHeight / r3 * Math.Sin((float)x / flatness * r3 + r3);
                h += offset;
                contour[x] = (int)h;
            }
        }
        void makeTexture()
        {
            Color[,] ground = tex2array(texGround);
            Color[] foreground = new Color[width * height];
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    if (y > contour[x]) {
                        foreground[x + y * width] = ground[x % texGround.Width, y % texGround.Height];
                    }
                    else {
                        foreground[x + y * width] = Color.Transparent;
                    }
                }
            }
            texFore = new Texture2D(device, width, height, false, SurfaceFormat.Color);
            texFore.SetData(foreground);
        }

        void updateRocket()
        {
            if (rocket.flying) {
                Vector2 gravity = new Vector2(0, 1);
                rocket.angle = (float)Math.Atan2(rocket.dir.X, -rocket.dir.Y);
                rocket.dir += gravity / 10.0f;
                rocket.pos += rocket.dir;

                Vector2 smokePos = rocket.pos;
                smokePos.X += rand.Next(10) - 5;
                smokePos.Y += rand.Next(10) - 5;
                smokeList.Add(smokePos);
            }
        }
        void updateParticles(GameTime t)
        {
            float now = (float)t.TotalGameTime.TotalMilliseconds;
            for (int i = particles.Count() - 1; i >= 0; --i)
            {
                Particle p = particles[i];
                float alive = now - p.tBirth;

                if (alive > p.maxAge)
                {
                    particles.RemoveAt(i);
                }
                else
                {
                    float age = alive / p.maxAge;
                    p.position = 0.5f * p.acceleration * age * age + p.direction * age + p.start;

                    float iAge = 1.0f - age;
                    p.col = new Color(new Vector4(iAge, iAge, iAge, iAge)); ;

                    Vector2 posCentre = p.position - p.start;
                    float dist = posCentre.Length();
                    p.scale = (50.0f + dist) / 200.0f;

                    particles[i] = p;
                }
            }
        }
        #endregion
        #region collision
        Color[,] tex2array(Texture2D tex)
        {
            Color[] cols = new Color[tex.Width * tex.Height];
            tex.GetData(cols);

            Color[,] ret = new Color[tex.Width, tex.Height];
            for (int x = 0; x < tex.Width; ++x)
            {
                for (int y = 0; y < tex.Height; ++y)
                {
                    ret[x, y] = cols[x + y * tex.Width];
                }
            }
            return ret;
        }
        void check(GameTime t)
        {
            Vector2 pos = collidesPlayers();
            if (pos.X > -1)
            {
                rocket.flying = false;
                smokeList = new List<Vector2>();
                explosion(pos, 10, 80, 2000, t);
                next();
            }
            if (collidesTerrain().X > -1)
            {
                rocket.flying = false;
                smokeList = new List<Vector2>();
                explosion(collidesTerrain(), 4, 30, 1000, t);
                next();
            }
            if (outScreen())
            {
                rocket.flying = false;
                smokeList = new List<Vector2>();
                next();
            }
        }
        private void next()
        {
             cPlayer = cPlayer + 1;
             cPlayer = cPlayer % num_players;
             while (!players[cPlayer].isAlive) {
                 cPlayer = ++cPlayer % num_players;
            }
        }
        CollData collide(Texture2D tex1, Matrix mat1, Texture2D tex2, Matrix mat2)
        {
            CollObj obj1 = new CollObj(tex2array(tex1), mat1);

            CollObj obj2 = new CollObj(tex2array(tex2), mat2);

            Matrix m = obj1.mat * Matrix.Invert(obj2.mat);

            int w1 = obj1.tex.GetLength(0);
            int h1 = obj1.tex.GetLength(1);
            for (int x1 = 0; x1 < w1; ++x1)
            {
                for (int y1 = 0; y1 < h1; ++y1)
                {
                    Vector2 pos1 = new Vector2(x1, y1);
                    Vector2 pos2 = Vector2.Transform(pos1, m);

                    int w2 = obj2.tex.GetLength(0);
                    int h2 = obj2.tex.GetLength(1);
                    int x2 = (int)pos2.X;
                    int y2 = (int)pos2.Y;

                    if (x2 >= 0 && x2 < w2 && y2 >= 0 && y2 < h2)
                    {
                        if (obj1.tex[x1, y1].A > 0 && obj2.tex[x2, y2].A > 0)
                        {
                            Vector2 coord = Vector2.Transform(pos1, obj1.mat);
                            return new CollData(false, coord);
                        }
                    }
                }
            }

            return new CollData(false, new Vector2(-1, -1));
        }
        Vector2 collidesTerrain()
        {
            Matrix mRocket = Matrix.CreateTranslation(-42, -240, 0) * Matrix.CreateRotationZ(rocket.angle) * Matrix.CreateScale(rocket.scale)
                               * Matrix.CreateTranslation(rocket.pos.X, rocket.pos.Y, 0);
            Matrix mTerrain = Matrix.Identity;
            return collide(texRocket, mRocket, texFore, mTerrain).pos;
        }
        Vector2 collidesPlayers()
        {
            Matrix mRocket = Matrix.CreateTranslation(-42, -240, 0) * Matrix.CreateRotationZ(rocket.angle) * Matrix.CreateScale(rocket.scale)
                               * Matrix.CreateTranslation(rocket.pos.X, rocket.pos.Y, 0);
            for (int i = 0; i < num_players; ++i)
            {
                Player p = players[i];
                if (p.isAlive)
                {
                    if (i != cPlayer)
                    {
                        int xp = (int)p.position.X;
                        int yp = (int)p.position.Y;

                        Matrix mCarriage = Matrix.CreateTranslation(0, -texCarriage.Height, 0) * Matrix.CreateScale(pScale)
                                           * Matrix.CreateTranslation(xp, yp, 0);
                        CollData pos = collide(texCarriage, mCarriage, texRocket, mRocket);
                        if (pos.collides)
                        {
                            p.isAlive = false;
                            return pos.pos;
                        }

                        Matrix mCannon = Matrix.CreateTranslation(-11, -50, 0) * Matrix.CreateRotationZ(p.angle)
                                         * Matrix.CreateScale(pScale) * Matrix.CreateTranslation(xp + 20, yp - 10, 0);
                        Vector2 point = collide(texCannon, mCannon, texRocket, mRocket).pos;
                        if (point.X > -1)
                        {
                            players[i].isAlive = false;
                            return point;
                        }
                    }
                }
            }
            return new Vector2(-1, -1);
        }
        bool outScreen()
        {
            return rocket.pos.Y > height || rocket.pos.X < 0 || rocket.pos.X > width;
        }
        #endregion
    }
}
