using System;

namespace FPS
{
#if WINDOWS || XBOX
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
       {
           /*try
           {*/
               using (FirstPerson game = new FirstPerson())
               {
                   game.Run();
               }
           /*}
           catch (Exception e)
           {
               Console.Write("Exception: " + e.ToString());
               Console.ReadKey();
           }*/
        }
    }
#endif
}

