package client;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import remote.IRemoteMath;

/**
 * This class retrieves a reference to the remote object from the RMI registry. It
 * invokes the methods on the remote object as if it was a local object of the type of the 
 * remote interface.
 *
 */
public class MathClient {

	public static void main(String[] args) {
		
		try {
			//Connect to the rmiregistry that is running on localhost
			Registry registry = LocateRegistry.getRegistry("localhost");
           
			//Retrieve the stub/proxy for the remote math object from the registry
			IRemoteMath remoteMath = (IRemoteMath) registry.lookup("Compute");
           
			//Call methods on the remote object as if it was a local object
			double addResult = remoteMath.add(5.0, 3.0);
			System.out.println("5.0 + 3.0 = " + addResult);
			double subResult = remoteMath.subtract(5.0, 3.0);
			System.out.println("5.0 - 3.0 = " + subResult);
			
		}catch(Exception e) {
			e.printStackTrace();
		}
		
	}
	
}
