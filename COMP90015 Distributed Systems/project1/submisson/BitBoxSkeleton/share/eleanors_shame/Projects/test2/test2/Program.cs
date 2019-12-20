using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace test
{
    class Foo
    {
        public int a;
    }
    class Bar: Foo
    {
        public int b;
    }
    class Baz: Foo
    {
        public int c;
    }
    class Program
    {
        static void Main(string[] args)
        {
            Bar ex = new Bar();
            ex.a = 1;
            ex.b = 2;

            Baz ay = new Baz();
            ay.a = 3;
            ay.c = 4;

            Foo fa = ex;

            if (fa is Bar)
            {
                Console.WriteLine("fa is Bar");
            }
            fa = ay;
            if (fa is Baz)
            {
                Console.WriteLine("fa is Baz");
            }
            Console.ReadLine();
        }
    }
}
