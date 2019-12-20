import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;

import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

public class SSLClient {
	
	public static void main(String[] arstring) {
		
		try {
			
			//Location of the Java keystore file containing the collection of 
			//certificates trusted by this application (trust store).
			System.setProperty("javax.net.ssl.trustStore", "clientKeyStore/myGreatName");
			
			
			//System.setProperty("javax.net.debug","all");
			
			//Create SSL socket and connect it to the remote server 
			SSLSocketFactory sslsocketfactory = (SSLSocketFactory) SSLSocketFactory.getDefault();
			SSLSocket sslsocket = (SSLSocket) sslsocketfactory.createSocket("localhost", 9999);

			//Create buffered reader to read input from the console
			InputStream inputstream = System.in;
			InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
			BufferedReader bufferedreader = new BufferedReader(inputstreamreader);

			//Create buffered writer to send data to the server
			OutputStream outputstream = sslsocket.getOutputStream();
			OutputStreamWriter outputstreamwriter = new OutputStreamWriter(outputstream);
			BufferedWriter bufferedwriter = new BufferedWriter(outputstreamwriter);

			String string = null;
			//Read line from the console
			while ((string = bufferedreader.readLine()) != null) {
				//Send data to the server
				bufferedwriter.write(string + '\n');
				bufferedwriter.flush();
			}
		} catch (Exception exception) {
			exception.printStackTrace();
		}
	}
}