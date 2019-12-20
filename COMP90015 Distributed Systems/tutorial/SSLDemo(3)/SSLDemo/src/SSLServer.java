
/* from www.javaworld.com */
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLSocket;

public class SSLServer {
	
	public static void main(String[] arstring) {
		
		//Specify the keystore details (this can be specified as VM arguments as well)
		//the keystore file contains an application's own certificate and private key
		System.setProperty("javax.net.ssl.keyStore","serverKeystore/aGreatName");
		//Password to access the private key from the keystore file
		System.setProperty("javax.net.ssl.keyStorePassword","comp90015");

		// Enable debugging to view the handshake and communication which happens between the SSLClient and the SSLServer
		System.setProperty("javax.net.debug","all");
		
		try {
			//Create SSL server socket
			SSLServerSocketFactory sslserversocketfactory = (SSLServerSocketFactory) SSLServerSocketFactory
					.getDefault();
			SSLServerSocket sslserversocket = (SSLServerSocket) sslserversocketfactory.createServerSocket(9999);
			
			//Accept client connection
			SSLSocket sslsocket = (SSLSocket) sslserversocket.accept();

			//Create buffered reader to read input from the client
			InputStream inputstream = sslsocket.getInputStream();
			InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
			BufferedReader bufferedreader = new BufferedReader(inputstreamreader);

			String string = null;
			//Read input from the client and print it to the screen
			while ((string = bufferedreader.readLine()) != null) {
				System.out.println(string);
			}
		} catch (Exception exception) {
			exception.printStackTrace();
		}
	}
}
