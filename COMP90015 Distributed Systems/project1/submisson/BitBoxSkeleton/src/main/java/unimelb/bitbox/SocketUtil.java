package unimelb.bitbox;

import unimelb.bitbox.util.Document;

import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * SocketUtil
 * Usage:
 * Define the different functions
 *
 * @author Tong Mu
 * create time 2019/5/2 - 9:57 PM
 */
public class SocketUtil {

    public static String getSocketHost(Socket socket) {
        return socket.getInetAddress().getHostName();
    }

    public static String getSocketIp(Socket socket) {
        return getSocketHost(socket) + ":" + socket.getPort();
    }

    public static void print(Document re, BufferedWriter out) throws IOException {
        out.write(re.toJson() + "\n");
        out.flush();
    }

}
