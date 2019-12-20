package tcpInteractive;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

public class tcpInteractiveServer {

	public static void main(String[] args) {
		
		ServerSocket listeningSocket = null;
		Socket clientSocket = null;
		
		try {
			//Create a server socket listening on port 4444
			listeningSocket = new ServerSocket(4444);
			int i = 0; //counter to keep track of the number of clients
			
			
			//Listen for incoming connections for ever 
			while (true) {
				System.out.println("Server listening on port 4444 for a connection");
				//Accept an incoming client connection request 
				clientSocket = listeningSocket.accept(); //This method will block until a connection request is received
				i++;
				System.out.println("Client conection number " + i + " accepted:");
				System.out.println("Remote Port: " + clientSocket.getPort());
				System.out.println("Remote Hostname: " + clientSocket.getInetAddress().getHostName());
				System.out.println("Local Port: " + clientSocket.getLocalPort());
				
				//Get the input/output streams for reading/writing data from/to the socket
				BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream(), "UTF-8"));
				BufferedWriter out = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream(), "UTF-8"));

				
				//Read the message from the client and reply
				//Notice that no other connection can be accepted and processed until the last line of 
				//code of this loop is executed, incoming connections have to wait until the current
				//one is processed unless...we use threads!
				String clientMsg = null;
				try {
				while((clientMsg = in.readLine()) != null) {
					System.out.println("Message from client " + i + ": " + clientMsg);
					out.write("Server Ack " + clientMsg + "\n");
					out.flush();
					System.out.println("Response sent");
				}}
				catch(SocketException e) {
					System.out.println("closed...");
				}
				clientSocket.close();
			}
		} catch (SocketException ex) {
			ex.printStackTrace();
		}catch (IOException e) {
			e.printStackTrace();
		} 
		finally {
			if(listeningSocket != null) {
				try {
					listeningSocket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
