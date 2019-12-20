using System;

namespace Physics
{
#if WINDOWS || XBOX
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            using (Physics game = new Physics())
            {
                game.Run();
            }
        }
    }
#endif
}

