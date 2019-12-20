package unimelb.bitbox;

import unimelb.bitbox.util.Configuration;
import unimelb.bitbox.util.Document;
import unimelb.bitbox.util.FileSystemManager;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Base64;
import java.util.HashSet;
import java.util.Set;
import java.util.logging.Logger;

import static unimelb.bitbox.ServerMain.peerList;
import static unimelb.bitbox.SocketUtil.getSocketHost;
import static unimelb.bitbox.SocketUtil.print;

/**
 * SocketHandler
 * Usage:
 * socket client listener & the handler of all kinds of Operation {@link Operation}
 *
 * @author Junjie Huang, Tong Mu
 * create time 30/04/2019
 */
public class SocketHandler implements Runnable {

    private static Logger log = Logger.getLogger(SocketHandler.class.getName());

    private Socket socket;

    private String ip;

    private Set<String> pathName = new HashSet<>();

    private FileSystemManager manager;

    private boolean needHandShake;

    /**
     * Usage:
     * Operation Table
     */
    private OperationMonitor[] monitorTable;

    public SocketHandler(Socket socket, FileSystemManager manager, boolean needHandShake) {
        this.socket = socket;
        this.manager = manager;
        this.needHandShake = needHandShake;
        // table driven
        monitorTable = new OperationMonitor[]{optConnectionRefused, optHandshakeReq, stubOperationMonitor, optFileCreateReq, stubOperationMonitor, optFileBytesReq, optFileBytesRes, optFileDeleteReq, stubOperationMonitor, optFileModifyReq, stubOperationMonitor, optDirectoryCreateReq, stubOperationMonitor, optDirectoryDeleteReq, stubOperationMonitor, stubOperationMonitor,};
    }

    @Override
    public void run() {
        // establish stream
        if (socket == null) {
            return;
        }
        try {
            BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8"));
            BufferedWriter output = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream(), "UTF-8"));
//            DataInputStream input = new DataInputStream(socket.getInputStream());
//            DataOutputStream output = new DataOutputStream(socket.getOutputStream());

            if (needHandShake) {
                log.info("perform handshake to " + socket.getPort());
                Document handShakeReq = new Document();
                handShakeReq.append("command", Operation.HANDSHAKE_REQUEST.name());
                Document hostPort = new Document();
                hostPort.append("host", ServerMain.host);
                hostPort.append("port", ServerMain.port);
                handShakeReq.append("hostPort", hostPort);
                ServerMain.peerList.add(socket);
                print(handShakeReq, output);
            }
            log.info("begin listen to socket " + socket.getPort());
            while (true) {
                String data;
                try {

                    data = input.readLine();
                    if (data.isEmpty()) {
                        break;
                    }
                } catch (Exception e) {
                    // connection closed or EOF
                    e.printStackTrace();
                    break;
                }

                Document request = Document.parse(data);
                String command = request.getString("command");
                // empty request, skip
                if (command == null) {
                    continue;
                }
                log.info(data);

                // map string to specific operation
                Operation operation = Operation.valueOf(command);
                OperationMonitor monitor = monitorTable[operation.ordinal()];
                monitor.run(request, output);
                // handle operation
                if (operation == Operation.CONNECTION_REFUSED || operation == Operation.INVALID_PROTOCOL) {
                    break;
                }

            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (!socket.isClosed()) {
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            ServerMain.removeWithIp(ip);
        }

        try {
            for (String path : pathName) {
                manager.cancelFileLoader(path);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Usage:
     * Use option table to manage the commands.
     *
     */
    @FunctionalInterface
    public interface OperationMonitor {

        void run(Document request, BufferedWriter output) throws Exception;

    }

    private OperationMonitor stubOperationMonitor = (request, output) -> {
    };

    /**
     * Usage
     * Try to connect to each peer in the peerlist
     *
     */
    private OperationMonitor optConnectionRefused = (request, output) -> {
        ArrayList<Document> peers = (ArrayList<Document>) request.get("peers");
        for (Document eachPeer : peers) {
            String host = eachPeer.getString("host");
            int port = eachPeer.getInteger("port");
            // try to connect to peers
            try {
                Socket socket = new Socket(host, port);
                log.info("try to connect to next peer: " + ip);
                // peerMap.put(ip, socket);
                new Thread(new SocketHandler(socket, manager, true)).run();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    /**
     * Usage
     * Class to create output for directory delete
     *
     */
    private OperationMonitor optDirectoryDeleteReq = (request, output) -> {
        String pathName = request.getString("pathName");
        boolean suc = manager.deleteDirectory(pathName);
        Document re = Document.parse(request.toJson());
        re.append("command", Operation.DIRECTORY_DELETE_RESPONSE.name());
        appendStatusToResult(re, suc, suc ? "directory deleted" : "there was a problem deleting the directory");
        print(re, output);
    };

    /**
     * Usage
     * Class to create output for directory create
     *
     */
    private OperationMonitor optDirectoryCreateReq = (request, output) -> {
        String pathName = request.getString("pathName");
        Document re = Document.parse(request.toJson());
        re.append("command", Operation.DIRECTORY_CREATE_RESPONSE.name());

        if (manager.isSafePathName(pathName)) {
            if (manager.dirNameExists(pathName)) {
                appendStatusToResult(re, false, "pathname already exists");
            } else {
                if (manager.makeDirectory(pathName)) {
                    appendStatusToResult(re, true, "directory created");
                } else {
                    appendStatusToResult(re, false, "there was a problem creating the directory");
                }
            }
        } else {
            appendStatusToResult(re, false, "unsafe pathname given");
        }
        print(re, output);
    };

    /**
     * Usage
     * Class to create output for file modify
     */
    private OperationMonitor optFileModifyReq = (request, output) -> {
        Document fileDescriptor = (Document) request.get("fileDescriptor");
        String md5 = fileDescriptor.getString("md5");
        long lastModified = fileDescriptor.getLong("lastModified");
        long fileSize = fileDescriptor.getLong("fileSize");
        String pathName = request.getString("pathName");

        Document re = Document.parse(request.toJson());
        re.append("command", Operation.FILE_MODIFY_RESPONSE.name());
        try {
            boolean canCreate = manager.isSafePathName(pathName) &&
                    manager.fileNameExists(pathName) &&
                    manager.modifyFileLoader(pathName, md5, lastModified);

            if (canCreate) {
                // handle the create-option
                // we have shortcut, no need to create
                appendStatusToResult(re, true, "file loader ready");
                print(re, output);
                if (!manager.checkShortcut(pathName)) {
                    if (!manager.checkWriteComplete(pathName)) {
                        // begin to read file
                        Document reqFile = buildFileBytesRequest(md5, lastModified, fileSize, pathName, 0, Math.min(fileSize, ServerMain.blockSize));
                        print(reqFile, output);
                    }
                }
                return;
            }
        } catch (Exception ex) {
            // error encounter when create
            ex.printStackTrace();
        }

        appendStatusToResult(re, false, "invalid parameters");
        print(re, output);
    };

    /**
     * Usage
     * Class for file delete
     */
    private OperationMonitor optFileDeleteReq = ((request, output) -> {
        Document fileDescriptor = (Document) request.get("fileDescriptor");
        String md5 = fileDescriptor.getString("md5");
        long lastModified = fileDescriptor.getLong("lastModified");
        // long fileSize = fileDescriptor.getLong("fileSize");
        String pathName = request.getString("pathName");
        // no need to do more check, file system will help us
        // Success is God’s will
        boolean suc = manager.deleteFile(pathName, lastModified, md5);
        Document res = Document.parse(request.toJson());
        res.append("command", Operation.FILE_DELETE_RESPONSE.name());
        appendStatusToResult(res, suc, suc ? "file deleted" : "there was a problem deleting the file");
        print(res, output);
    });

    /**
     * Usage
     * Class for write file
     */
    private OperationMonitor optFileBytesRes = (request, output) -> {
        Document fileDescriptor = (Document) request.get("fileDescriptor");
        String md5 = fileDescriptor.getString("md5");
        long lastModified = fileDescriptor.getLong("lastModified");
        long fileSize = fileDescriptor.getLong("fileSize");
        String pathName = request.getString("pathName");
        long position = request.getLong("position");
        long length = request.getLong("length");
        byte[] content = Base64.getDecoder().decode(request.getString("content"));
        boolean status = request.getBoolean("status");
        if (status) {
            // save to file
            boolean suc = manager.writeFile(pathName, ByteBuffer.wrap(content), position);
            if (suc) {
                if (!manager.checkWriteComplete(pathName)) {
                    // more page
                    long remain = fileSize - position - length;
                    Document morePageReq = buildFileBytesRequest(md5, lastModified, fileSize, pathName, position + length, Math.min(remain, ServerMain.blockSize));
                    print(morePageReq, output);
                } else {
                    this.pathName.remove(pathName);
                }
            } else {
                throw new IOException("fail to write file");
            }
        }
    };

    /**
     * Usage
     * Class to create output for getting content
     */
    private OperationMonitor optFileBytesReq = (request, output) -> {
        Document fileDescriptor = (Document) request.get("fileDescriptor");
        String md5 = fileDescriptor.getString("md5");
        long position = request.getLong("position");
        long length = request.getLong("length");

        ByteBuffer byteBuffer = manager.readFile(md5, position, length);
        Document re = Document.parse(request.toJson());
        re.append("command", Operation.FILE_BYTES_RESPONSE.name());
        if (byteBuffer != null) {
            // encode
            re.append("content", Base64.getEncoder().encodeToString(byteBuffer.array()));
            appendStatusToResult(re, true, "successful read");
        } else {
            // no such file
            appendStatusToResult(re, false, "unsuccessful read");
        }

        print(re, output);
    };


    /**
     * Usage
     * Class to create output for file create
     */
    private OperationMonitor optFileCreateReq = (request, output) -> {
        Document fileDescriptor = (Document) request.get("fileDescriptor");
        String md5 = fileDescriptor.getString("md5");
        long lastModified = fileDescriptor.getLong("lastModified");
        long fileSize = fileDescriptor.getLong("fileSize");
        String pathName = request.getString("pathName");

        // we simply jsonfy the request and then un-jsonfy to copy the object
        Document re = Document.parse(request.toJson());
        re.append("command", Operation.FILE_CREATE_RESPONSE.name());
        // re.append("status", true);
        // determine if can be created or not
        boolean canCreate = false;
        try {
            canCreate = manager.isSafePathName(pathName) &&
                    !manager.fileNameExists(pathName, md5) &&
                    manager.createFileLoader(pathName, md5, fileSize, lastModified);

            if (canCreate) {
                // handle the create-option
                // we have shortcut, no need to create
                if (manager.checkShortcut(pathName)) {
                    appendStatusToResult(re, true, "create with short-cut");
                } else {
                    appendStatusToResult(re, true, "file loader ready");
                    print(re, output);
                    // begin to read file
                    Document reqFile = buildFileBytesRequest(md5, lastModified, fileSize, pathName, 0, Math.min(fileSize, ServerMain.blockSize));
                    print(reqFile, output);
                    return;
                }
            }

        } catch (Exception ex) {
            // error encounter when create
            ex.printStackTrace();
        }
        if (!canCreate) {
            appendStatusToResult(re, false, "invalid parameters");
        }
        print(re, output);
    };

    /**
     * Usage:
     * Append the status to final result
     * @param re
     * @param status
     * @param message
     */
    private void appendStatusToResult(Document re, boolean status, String message) {
        re.append("status", status);
        re.append("message", message);

    }

    /**
     * Usage
     * Class to create output for hand shake
     */
    private OperationMonitor optHandshakeReq = (request, output) -> {
        Document document = (Document) request.get("hostPort");
        String host = document.getString("host");
        int port = (int) document.getLong("port");

        Document re = new Document();
        // check pool size
        if (peerList.size() + 1 > Integer.parseInt(Configuration.getConfigurationValue("maximumIncommingConnections"))) {
            // full, close
            re.append("command", Operation.CONNECTION_REFUSED.name());
            re.append("message", "connection limit reached");
            ArrayList<Document> documents = new ArrayList<>(peerList.size());

            for (Socket socket : peerList) {
                Document socketDoc = new Document();
                socketDoc.append("host", getSocketHost(socket));
                socketDoc.append("port", socket.getPort());
                documents.add(socketDoc);
            }

            re.append("peers", documents);

        } else {
            re.append("command", Operation.HANDSHAKE_RESPONSE.name());
            Document hostPort = new Document();
            hostPort.append("host", ServerMain.host);
            hostPort.append("port", ServerMain.port);
            re.append("hostPort", hostPort);


            // normal, establish peer connection
            ip = host + ":" + port;
            // if established already, return directly
            log.info(ServerMain.peerList + " " + ip);
            if (!ServerMain.checkAlreadyConnect(ip)) {
                Socket newPeer = new Socket(host, port);
                peerList.add(newPeer);
                new Thread(new SocketHandler(newPeer, manager, false)).start();
            } else {
                log.info("already in peer, close");
                // close
                re.append("command", Operation.INVALID_PROTOCOL.name());
                re.append("message", "don't repeat to join the network");
                print(re, output);
                socket.close();
                return;
            }
        }

        print(re, output);
    };

    /**
     * Usage
     * build the file bytes request to send
     * @param md5
     * @param lastModified
     * @param fileSize
     * @param pathName
     * @param position
     * @param length
     * @return
     */
    private Document buildFileBytesRequest(String md5, long lastModified, long fileSize, String pathName, long position, long length) {
        this.pathName.add(pathName);
        Document req = new Document();
        req.append("command", Operation.FILE_BYTES_REQUEST.name());
        Document fileDescriptor = new Document();
        fileDescriptor.append("md5", md5);
        fileDescriptor.append("lastModified", lastModified);
        fileDescriptor.append("fileSize", fileSize);
        req.append("fileDescriptor", fileDescriptor);
        req.append("pathName", pathName);
        req.append("position", position);
        req.append("length", length);
        return req;
    }


}