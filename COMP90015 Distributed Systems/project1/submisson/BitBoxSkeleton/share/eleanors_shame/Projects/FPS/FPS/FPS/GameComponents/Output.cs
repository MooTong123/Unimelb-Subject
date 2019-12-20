using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FPS.GameComponents
{
    public static class Output
    {
        public static void Write(string caller, string str)
        {
            if (Global.Write)
            {
                Console.WriteLine(caller);
                Console.WriteLine(str);
            }
        }
    }
}
