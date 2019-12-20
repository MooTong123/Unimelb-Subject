

// Code adapted from Tutorialspoint


public class ThreadSync {
	   public static void main(String args[]) {
		      PrintDemo PD = new PrintDemo();

		      ThreadDemo T1 = new ThreadDemo( "Thread - 1 ", PD );
		      ThreadDemo T2 = new ThreadDemo( "Thread - 2 ", PD );

		      T1.start();
		      T2.start();

		      // wait for threads to end
		      try {
		         T1.join();
		         T2.join();
		      }catch( Exception e) {
		         System.out.println("Interrupted");
		      }
		   }
}


