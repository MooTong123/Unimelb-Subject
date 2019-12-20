import java.util.concurrent.ThreadLocalRandom;

class PrintDemo {
   
	public void printCount(String threadName) {
      try {
         for(int i = 5; i > 0; i--) {
            System.out.println("Thread "+threadName+" Counter   ---   "  + i );
            
            // Make the thread sleep from between 100 to 999 ms
            int randomSleepTime = ThreadLocalRandom.current().nextInt(100,1000);
            Thread.sleep(randomSleepTime);
         }
      }catch (Exception e) {
         System.out.println("Thread  interrupted.");
      }
   }
}