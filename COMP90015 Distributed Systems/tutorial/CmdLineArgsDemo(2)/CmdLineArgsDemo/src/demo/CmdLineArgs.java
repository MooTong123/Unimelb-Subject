package demo;

//Remember to add the args4j jar to your project's build path 
import org.kohsuke.args4j.Option;

//This class is where the arguments read from the command line will be stored
//Declare one field for each argument and use the @Option annotation to link the field
//to the argument name, args4J will parse the arguments and based on the name,  
//it will automatically update the field with the parsed argument value
public class CmdLineArgs {

	@Option(required = true, name = "-h", aliases = {"--host"}, usage = "Hostname")
	private String host;
	
	@Option(required = false, name = "-p", usage = "Port number")
	private int port = 4444;

	public String getHost() {
		return host;
	}

	public int getPort() {
		return port;
	}
	
}
