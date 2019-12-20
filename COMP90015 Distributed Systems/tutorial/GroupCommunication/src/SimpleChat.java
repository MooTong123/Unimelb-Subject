/**
 * 
 */

/**
 * @author IRUM
 *
 */
import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.jgroups.JChannel;
import org.jgroups.Message;
import org.jgroups.ReceiverAdapter;
import org.jgroups.View;


public class SimpleChat extends ReceiverAdapter {

    JChannel channel;
    String user_name=System.getProperty("user.name", "n/a");

    private void start() throws Exception {

        channel=new JChannel(); // use the default config, udp.xml
        channel.setReceiver(this);
        channel.connect("ChatCluster");
        eventLoop();        
        channel.close();
    }
    /**
     * send Message to other in the group
     */
     private void eventLoop() {

            BufferedReader in=new BufferedReader(new InputStreamReader(System.in));

            while(true) {

                try {

                    System.out.print("> "); System.out.flush();

                    String line=in.readLine().toLowerCase();

                    if(line.startsWith("quit") || line.startsWith("exit"))

                        break;

                    line="[" + user_name + "] " + line;
                    
                    Message msg=new Message(null, line);// null means broadcast to group
                    channel.send(msg);

                }

                catch(Exception e) {

                }

            }


    }
/**    The two methods in 'Receive Adaptor' are overriden here
 *     viewAccepted() and receive() 
 */
     public void viewAccepted(View new_view) {

    	    System.out.println("** view: " + new_view);

    	}


    	public void receive(Message msg) {

    	    System.out.println(msg.getSrc() + ": " + msg.getObject());

    	}
     
     
    public static void main(String[] args) throws Exception {

        new SimpleChat().start();

    }

}

