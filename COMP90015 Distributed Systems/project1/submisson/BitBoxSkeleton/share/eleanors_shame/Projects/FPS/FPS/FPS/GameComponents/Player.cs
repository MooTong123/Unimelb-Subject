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

using FPS.Entities;
using FPS.Effects;

namespace FPS.GameComponents
{
    public class Player : Unit
    {
        public enum States
        {
            none,
            swing,
            block
        }
        States state;
        public States State
        {
            get { return state; }
        }

        Unit target;
        public Unit Target
        {
            get { return target; }
        }

        public class Sword
        {
            private readonly Quaternion swingRot = new Quaternion(-0.75f, 0, 0, 1);
            private readonly Quaternion blockRot = new Quaternion(-1, 1, 0, 1);

            private Quaternion rot;

            protected Player player;

            protected Model model;

            private bool swinging;
            private bool blocking;

            public Sword(Player player)
            {
                this.player = player;
                model = player.game.Content.Load<Model>("sword");

                rot = Quaternion.Identity;
            }

            public Quaternion Get()
            {
                if (swinging) {
                    return swingRot;
                }
                if (blocking) {
                    return blockRot;
                }
                return Quaternion.Identity;
            }

            protected virtual bool damage(Entity e)
            {
                return player.game.level.hurt(e, 10, Damage.Physical);
            }

            public void Update()
            {
                if (player.game.ui.Item is FPS.Items.InventoryItems.Sword)
                {
                    if (Input.MouseState.LeftButton == ButtonState.Released)
                    {
                        swinging = false;
                        player.state = States.none;
                    } if (Input.MouseState.RightButton == ButtonState.Released)
                    {
                        blocking = false;
                        player.state = States.none;
                    }
                    if (Input.MouseState.LeftButton == ButtonState.Pressed && blocking == false)
                    {
                        if (swinging == false)
                        {
                            Entity e = player.game.level.entityCollision(player.look, 128);
                            if (e != null)
                            {
                                effect(e);
                                if (damage(e))
                                {
                                    player.target = null;
                                }
                                else
                                {
                                    player.target = e;
                                }
                            }
                        }
                        swinging = true;
                        player.state = States.swing;
                    }

                    if (Input.MouseState.RightButton == ButtonState.Pressed)
                    {
                        blocking = true;
                        player.state = States.block;
                    }
                }
                else
                {
                    swinging = false;
                    blocking = false;
                    player.state = States.none;
                }

                rot = Quaternion.Lerp(rot, Get(), 0.4f);
            }

            protected virtual void effect(Unit target) { }

            public void Draw()
            {
                if (!(player.game.ui.Item is Items.InventoryItems.Sword))
                {
                    return;
                }
                Vector3 swordPos = player.position + player.look.Direction * 10;
                Drawing.drawModel(model, Matrix.CreateScale(0.2f)
                    * Matrix.CreateRotationY(MathHelper.ToRadians(90))
                    * Matrix.CreateRotationX(MathHelper.ToRadians(30))
                    * Matrix.CreateFromQuaternion(rot)
                    * Matrix.CreateRotationX(player.yRot) * Matrix.CreateRotationY(player.xzRot)
                    * Matrix.CreateTranslation(swordPos)
                    * Matrix.CreateTranslation(Vector3.Transform(new Vector3(5, -1, 0), Matrix.CreateRotationX(player.yRot) * Matrix.CreateRotationY(player.xzRot))), player.game);
            }
        }
        public class MooSword : Sword
        {
            public MooSword(Player p)
                : base(p)
                {
                    model = p.game.Content.Load<Model>("moosword");
                }

            protected override void effect(Unit target)
            {
                target.applyEffect(new Dot(player.game));
            }
        }
        public Sword sword;

        public void replaceWeapon(Sword weapon)
        {
            sword = weapon;
        }

        Vector3 size = new Vector3(Global.TileSize / 2, Global.TileSize * 2 - Global.TileSize / 4, Global.TileSize / 2);

        public Matrix view;
        public Matrix projection;
        public Vector3 Up
        {
            get { return up; }
        }
                
        // head bob
        float sin;
        float t;

        public Vector3 Position
        {
            get
            {
                return position;
            }
        }

        Vector3 to;
        Vector3 up = new Vector3();
        Vector3 position;
        Vector3 posPrevious;

        public float xzRot = 0;
        public float yRot = 0;
        float maxHeight = 0;
        float yspeed = 0;

        public const float gravity = Global.TileSize / 140f;
        const float rotSpeed = 0.01f;
        const float moveSpeed = Global.TileSize / 10f;
        const float initialSpeed = (Global.TileSize / 10f) + 1 / (gravity * 100f);

        public Ray look;
        
        bool falling = false;
        float health = 100;
        public override float Health
        {
            get { return health; }
        }

        public void Effects(Texture2D texture)
        {
            game.effect.CurrentTechnique = game.effect.Techniques["Textured"];
            game.effect.Parameters["xWorld"].SetValue(Matrix.Identity);
            game.effect.Parameters["xView"].SetValue(game.player.view);
            game.effect.Parameters["xProjection"].SetValue(game.player.projection);
            game.effect.Parameters["xTexture"].SetValue(texture);
            game.effect.Parameters["xEnableLighting"].SetValue(true);
            game.effect.Parameters["xAmbient"].SetValue(0.5f);
            Vector3 lightDir = new Vector3(3, -2, 5);
            lightDir.Normalize();
            game.effect.Parameters["xLightDirection"].SetValue(lightDir);
        }

        public override bool hurt(float amount, Damage type)
        {
            amount *= 1 - resist[type];
            new DamageText(game, (int)amount, DamageText.Units.Player);
            health -= amount;
            if (health <= 0)
            {
                System.Windows.Forms.MessageBox.Show("You died.");
                game.Exit();
            }
            return true;
        }
        public void heal(float amount)
        {
            if (health + amount >= MaxHealth)
            {
                amount = MaxHealth - health;
            }
            new HealText(game, (int)amount, HealText.Units.Player);
            health += amount;
        }

        public Player(Game game)
            : base(game)
        {
            resist[Damage.Nature] = 75;

            MaxHealth = 100;
            name = "Bob";

            this.game = game as FirstPerson;

            Level level = this.game.level;
            position = new Vector3(129) + size;
            posPrevious = position;
            
            yRot = 0;

            sword = new Sword(this);

            view = Matrix.CreateLookAt(position, to, new Vector3(0, 1, 0));
            projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver4, game.GraphicsDevice.Viewport.AspectRatio, 0.2f, 32000);

            bb = new BoundingBox(position - size, position);
        }

        public override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            base.LoadContent();
        }

        private Vector3 movement()
        {
            Vector3 move = new Vector3(0, 0, 0);
            if (Input.State.IsKeyDown(Keys.W))
            {
                move += new Vector3(0, 0, -1);
            }
            if (Input.State.IsKeyDown(Keys.S))
            {
                move += new Vector3(0, 0, 1);
            }
            if (Input.State.IsKeyDown(Keys.A))
            {
                move += new Vector3(-1, 0, 0);
            }
            if (Input.State.IsKeyDown(Keys.D))
            {
                move += new Vector3(1, 0, 0);
            }
            if (state == States.block)
            {
                move /= 2;
            }
            return move;
        }
        private void move()
        {
            Vector3 move = movement();
            move.Y += yspeed;

            Vector3 nextPos = moveSpeed * Vector3.Transform(move, Matrix.CreateRotationY(xzRot));
            if (Input.State.IsKeyDown(Keys.Y))
            {
                position += nextPos;
                bb.Min = position - size;
                bb.Max = position;
                return;
            }

            posPrevious = position;
            position.X += nextPos.X;
            bb.Min = position - size;
            bb.Max = position;
            BoundingBox? result = game.level.blockCollision(bb);
            if (result != null) {
                position = posPrevious;
            }

            posPrevious = position;
            position.Y += nextPos.Y;
            bb.Min = position - size;
            bb.Max = position;
            result = game.level.blockCollision(bb);
            if (result != null) {
                position = posPrevious;

                falling = false;
                yspeed = 0;
            }

            posPrevious = position;
            position.Z += nextPos.Z;
            bb.Min = position - size;
            bb.Max = position;
            result = game.level.blockCollision(bb);
            if (result != null) {
                position = posPrevious;
            }
        }
        private void input()
        {
            if (Input.State.IsKeyDown(Keys.Escape)) {
                game.Exit();
            }
            if (Input.keyDown(Keys.T))
            {
                game.ui.addItem(new Items.InventoryItems.MooSword(game));
            }

            if (Input.MouseState.LeftButton == ButtonState.Pressed && Input.LastMouse.LeftButton == ButtonState.Released)
            {
                if (!(game.ui.Item is Items.InventoryItems.Sword) && game.ui.Item != null)
                {
                    game.ui.Item.use();
                }
            }

            xzRot -= rotSpeed * (Input.MouseState.X - Input.MouseStart.X) / 2;
            yRot -= rotSpeed * (Input.MouseState.Y - Input.MouseStart.Y) / 2;
            if (yRot > MathHelper.PiOver2) {
                yRot = MathHelper.PiOver2;
            }
            if (yRot < -MathHelper.PiOver2) {
                yRot = -MathHelper.PiOver2;
            }
            if (xzRot >= MathHelper.TwoPi) {
                xzRot = 0;
            }
            if (xzRot <= -MathHelper.TwoPi) {
                xzRot = 0;
            }

            if (Input.State.IsKeyDown(Keys.Space) && !falling) {
                maxHeight = 0;
                falling = true;
                yspeed = initialSpeed;
            }

            if (Input.State.IsKeyDown(Keys.R))
            {
                falling = false;
            }
            if (falling) {
                yspeed -= gravity;
            }
        }
        private void checkCollision()
        {
            bb.Min = position - size;
            bb.Max = position;
            BoundingBox underPlayer = bb;
            underPlayer.Max.Y -= 1f;
            underPlayer.Min.Y -= 1f;
            BoundingBox? result = game.level.blockCollision(underPlayer);
            if (result == null) {
                falling = true;
            }
        }
        private void updateCamera()
        {
            if (position != posPrevious && !falling) {
                t++;
                sin = (float)Math.Sin(t / 10f);
                sin /= 25;
            }

            Vector3 pos = position;
            pos.Y += sin;
            Matrix rot = Matrix.CreateRotationX(yRot) * Matrix.CreateRotationY(xzRot);
            to = pos + Vector3.Transform(new Vector3(0, 0, -1), rot);
            to.Y += sin;
            up = Vector3.Transform(new Vector3(0, 1, 0), rot);

            look.Position = pos;
            Vector3 dir = to - pos;
            dir.Normalize();
            look.Direction = dir;

            view = Matrix.CreateLookAt(pos, to, up);
        }
        public override void Update(GameTime gameTime)
        {
            if (Input.keyDown(Keys.P))
            {
                game.ui.removeItem(new Items.InventoryItems.Mush(game), 1);
            }
            if (position.Y > maxHeight) {
                maxHeight = position.Y;
            }
            move();
            input();
            checkCollision();
            updateCamera();

            sword.Update();

            if (position.Y < 0)
            {
                MaxHealth = 100;
                name = "Bob";

                this.game = game as FirstPerson;

                Level level = this.game.level;
                position = new Vector3(129) + size;
                posPrevious = position;

                yRot = 0;

                sword = new Sword(this);

                view = Matrix.CreateLookAt(position, to, new Vector3(0, 1, 0));
                projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver4, game.GraphicsDevice.Viewport.AspectRatio, 0.2f, 32000);

                bb = new BoundingBox(position - size, position);
            }

            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            RasterizerState rs = new RasterizerState();
            //rs.CullMode = CullMode.None;
            game.GraphicsDevice.RasterizerState = rs;

            sword.Draw();

            base.Draw(gameTime);
        }
    }
}
