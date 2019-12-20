
1. Start the rmiregistry by using the command:
	
		rmiregistry [port]
  
   This will start a remote object registry on the specified port on the current host.
   
2. Start the RMI server by running the RMIServer class with the following VM arguments:
		-Djava.rmi.server.codebase=file:<yourProjectLocation>/RMIMathServerExample/bin/

3. Start the client by running the MathClient class with the following VM arguments:
		-Djava.rmi.server.codebase=file:<yourProjectLocation>/RMIMathServerExample/bin/
	