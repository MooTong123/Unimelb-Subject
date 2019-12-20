package demo;

import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;

//This class just contains the main method, but notice that both classes can be merged into a 
//single one if preferred.
public class Demo {

	public static void main(String[] args) {
		
		//Object that will store the parsed command line arguments
		CmdLineArgs argsBean = new CmdLineArgs();
		
		//Parser provided by args4j
		CmdLineParser parser = new CmdLineParser(argsBean);
		try {
			
			//Parse the arguments
			parser.parseArgument(args);
			
			//After parsing, the fields in argsBean have been updated with the given
			//command line arguments
			System.out.println("Host: " + argsBean.getHost());
			System.out.println("Port: " + argsBean.getPort());
			
		} catch (CmdLineException e) {
			
			System.err.println(e.getMessage());
			
			//Print the usage to help the user understand the arguments expected
			//by the program
			parser.printUsage(System.err);
		}
		
		
	}

}
