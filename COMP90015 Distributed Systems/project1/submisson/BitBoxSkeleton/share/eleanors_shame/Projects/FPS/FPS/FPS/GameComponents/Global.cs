using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FPS.GameComponents
{
    public static class Global
    {
        private static int _tileSize;
        private static bool _write;

        public static int tileSize
        {
            get { return _tileSize; }
        }
        public static bool write
        {
            get { return _write; }
        }

        static Global()
        {
            _tileSize = 32;
            _write = true;
        }
    }
}
