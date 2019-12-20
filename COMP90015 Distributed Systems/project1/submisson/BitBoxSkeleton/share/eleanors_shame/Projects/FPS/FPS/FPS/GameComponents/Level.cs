using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using FPS.Blocks;
using FPS.Entities;

namespace FPS.GameComponents
{
    public class Level : Microsoft.Xna.Framework.DrawableGameComponent
    {
        private int width;
        private int height;
        private int yheight;

        private FirstPerson game;

        public int Width
        {
            get { return width; }
        }
        public int Height
        {
            get { return height; }
        }
        public int yHeight { get { return yheight; } }

        private List<Entity> objects = new List<Entity>();
        private List<List<List<Block>>> chunks = new List<List<List<Block>>>();

        struct ObjInfo
        {
            public System.Drawing.Color col;
            public int x;
            public int z;
        }
        private List<List<List<ObjInfo>>> getBitmapChunks(System.Drawing.Bitmap bitmap)
        {
            int chunksX = bitmap.Width / Global.ChunkSize;
            int chunksZ = bitmap.Height / Global.ChunkSize;

            List<List<List<ObjInfo>>> ret = new List<List<List<ObjInfo>>>();
            
            for (int xChunk = 0; xChunk < chunksX; ++xChunk)
            {
                for (int zChunk = 0; zChunk < chunksZ; ++zChunk)
                {
                    ret.Add(new List<List<ObjInfo>>());
                    for (int x = xChunk * Global.ChunkSize; x < (xChunk + 1) * Global.ChunkSize; ++x)
                    {
                        ret[xChunk * chunksX + zChunk].Add(new List<ObjInfo>());
                        for (int z = zChunk * Global.ChunkSize; z < (zChunk + 1) * Global.ChunkSize; ++z)
                        {
                            ObjInfo o = new ObjInfo();
                            o.col = bitmap.GetPixel(x, z);

                            // x/z were negative causing collision issues
                            o.x = (x + 1) * Global.TileSize;
                            o.z = (z + 1) * Global.TileSize;
                            ret[xChunk * chunksX + zChunk][x - xChunk * Global.ChunkSize].Add(o);
                        }
                    }
                }
            }
            return ret;
        }

        public Level(Game game)
            : base(game)
        {
            this.game = game as FirstPerson;
            List<System.Drawing.Bitmap> bitmaps = new List<System.Drawing.Bitmap>();
            try
            {
                for (int i = 0; i < System.IO.Directory.GetFiles(@"level\").Length; ++i)
                {
                    bitmaps.Add(new System.Drawing.Bitmap(@"level\level" + i.ToString() + ".bmp"));
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("An exception has been encountered from which the program cannot recover. Please contact" +
                                  " the application vendor.\n-----------------------------------------");
                Console.WriteLine(e.ToString());
                Console.ReadKey();
                game.Exit();
                game.Components.Remove(this);
                return;
            }
            yheight = bitmaps.Count;
            width = bitmaps[0].Width;
            height = bitmaps[0].Height;

            for (int i = 0; i < width / Global.ChunkSize + 1; ++i)
            {
                chunks.Add(new List<List<Block>>());
                for (int j = 0; j < height / Global.ChunkSize + 1; ++j)
                {
                    chunks[i].Add(new List<Block>());
                }
            }

            for (int y = 0; y < yheight; ++y)
            {
                List<List<List<ObjInfo>>> cs = getBitmapChunks(bitmaps[y]);
                int i = 0;
                for (int cx = 0; cx < width / Global.ChunkSize; ++cx)
                {
                    for (int cz = 0; cz < height / Global.ChunkSize; ++cz)
                    {
                        for (int x = 0; x < Global.ChunkSize; ++x) {
                            for (int z = 0; z < Global.ChunkSize; ++z)
                            {
                                ObjInfo o = cs[cz * height / Global.ChunkSize + cx][x][z];
                                Vector3 pos = new Vector3(o.x,
                                                          y * Global.TileSize,
                                                          o.z);
                                Collidable obj = Factory.Make(o.col, game, pos);
                                if (obj != null)
                                {
                                    if (obj is Block)
                                    {
                                        chunks[cx][cz].Add(obj as Block);
                                        Output.Write("Level.Level", "Block at chunk (" + (cx).ToString() + ", " + (cz).ToString() + ")");
                                        Output.Write("Level.Level", "Position: (" + pos.X.ToString() + ", " + pos.Y.ToString() + ", " + pos.Z.ToString() + ")");
                                    }
                                    else
                                    {
                                        objects.Add(obj as Entity);
                                    }
                                }
                            }
                        }
                        ++i;
                    }
                }
            }
        }

        public BoundingBox? blockCollision(BoundingBox bb)
        {
            foreach (Block b in chunks[(int)(bb.Max.Z / Global.TileSize / Global.ChunkSize)]
                                      [(int)(bb.Max.X / Global.TileSize / Global.ChunkSize)])
            {
                if (b.Box.Intersects(bb))
                {
                    return b.Box;
                }
            }
            return null;
        }

        public Entity entityCollision(BoundingBox bb)
        {
            foreach (Collidable c in objects)
            {
                if (c is Entity && c.Box.Intersects(bb))
                {
                    return c as Entity;
                }
            }
            return null;
        }

        internal struct Collision
        {
            public float distance;
            public Collidable obj;
        }
        public Collidable firstCollision(Ray ray, float range)
        {
            List<Collidable> objs = new List<Collidable>();
            foreach (List<List<Block>> lA in chunks)
            {
                foreach (List<Block> lB in lA)
                {
                    foreach (Block b in lB)
                    {
                        objs.Add(b);
                    }
                }
            }
            objs.Concat(objects);
            List<Collision> intersections = new List<Collision>();

            foreach (Collidable c in objects) {
                float? res = ray.Intersects(c.Box);
                if (res != null && (res.GetValueOrDefault() < range || range == 0)) {
                    Collision obj;
                    obj.distance = res.GetValueOrDefault();
                    obj.obj = c;

                    intersections.Add(obj);
                }
            }

            if (intersections.Count == 0) {
                return null;
            }

            intersections.Sort(
                                delegate(Collision a, Collision b)
                                {
                                    return a.distance.CompareTo(b.distance);
                                }
            );
            return intersections[0].obj;
        }
        public Entity entityCollision(Ray ray, float range)
        {
            Collidable res = firstCollision(ray, range);
            if (res is Entity)
            {
                return res as Entity;
            }
            return null;
        }
        public Block blockCollision(Ray ray, float range)
        {
            Collidable res = firstCollision(ray, range);
            if (res is Block) {
                return res as Block;
            }
            return null;
        }

        public bool hurt(Entity entity, int amount, Damage type)
        {
            for (int i = objects.Count - 1; i >= 0; --i)
            {
                if (objects[i] == entity)
                {
                    if (entity.hurt(amount, type))
                    {
                        objects.RemoveAt(i);
                        return true;
                    }
                    return false;
                }
            }
            return false;
        }
    }
}
