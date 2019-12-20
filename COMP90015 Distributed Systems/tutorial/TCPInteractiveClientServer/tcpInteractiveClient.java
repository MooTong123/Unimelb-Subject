package tcpInteractive;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

//Interactive client that reads input from the command line and sends it to 
//a server 
public class tcpInteractiveClient {

	public static void main(String[] args) {

		Socket socket = null;
		try {
			// Create a stream socket bounded to any port and connect it to the
			// socket bound to localhost on port 4444
			socket = new Socket("localhost", 4444);
			System.out.println("Connection established");

			// Get the input/output streams for reading/writing data from/to the socket
			BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8"));
			BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), "UTF-8"));

			Scanner scanner = new Scanner(System.in);
			String inputStr = null;

			//While the user input differs from "exit"
			while (!(inputStr = scanner.nextLine()).equals("exit")) {
				
				// Send the input string to the server by writing to the socket output stream
				out.write(inputStr + "\n");
				out.flush();
				System.out.println("Message sent");
				
				// Receive the reply from the server by reading from the socket input stream
				String received = in.readLine(); // This method blocks until there
													// is something to read from the
													// input stream
				System.out.println("Message received: " + received);
			}
			
			scanner.close();
			
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			// Close the socket
			if (socket != null) {
				try {
					socket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

	}

}
