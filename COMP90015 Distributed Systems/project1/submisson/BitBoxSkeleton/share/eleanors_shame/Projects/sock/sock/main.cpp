/*#include <Windows.h>
#include <WinSock.h>
#include <iostream>
#include <cstdlib>

void report_error (int code, const char* func);
void leave ();
int main ()
{
	WORD version = MAKEWORD(1, 1);
	WSADATA data;
	WSAStartup(version, &data);
	
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {

		report_error(WSAGetLastError(), "main() -> socket()");
		leave();
	}

	SOCKADDR_IN info;
	info.sin_family = AF_INET;
	info.sin_addr.s_addr = INADDR_ANY;
	info.sin_port = htons(8888);

	if (bind(sock, (sockaddr*)&info, sizeof(sockaddr)) == SOCKET_ERROR) {
		report_error(WSAGetLastError(), "main() -> bind()");
		leave();
	}
	
	if (listen(sock, 10) == SOCKET_ERROR) {
		report_error(WSAGetLastError(), "main() -> listen()");
		leave();
	}

	SOCKET client;
	client = accept(sock, 0, 0);
	if (client == INVALID_SOCKET) {
		report_error(WSAGetLastError(), "main() -> accept()");
		leave();
	}

	closesocket(client);
	closesocket(sock);

	WSACleanup();
	return 0;
}

void report_error (int code, const char* func)
{
	std::cout << "Call to '" << func << "' returned error " << code << "\n";
}
void leave ()
{
	WSACleanup();
	exit(1);
}*/

#include <39/39.h>
#include <iostream>

int main ()
{
	init39();
	t_socket sock = tcpconnect("plasticineguy.comli.com", 80, 1);
	
	clearbuffer();
	writechars("GET /ip.php HTTP/1.0\r\nFrom: plasticineguy@gmail.com\r\nUser-Agent: HTTPTool/1.0");
	sendmessage(sock, "plasticineguy.comli.com", 80);
	receivemessage(sock);
	char* msg = readchars(1000);
	std::cout << msg << "\n";
	std::cin.get();
	end39();
	return 0;
}