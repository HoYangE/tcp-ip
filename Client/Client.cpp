#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 512

using namespace std;

void ErrOutput(const char* errMassage);

int main()
{
	WSADATA wsaData;
	SOCKET connectSocket;
	SOCKADDR_IN serverAddr;

	char massage[BUFSIZE];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrOutput("WASStartup() Error!");

	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET)
		ErrOutput("socket() Error!");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(12345);

	if (connect(connectSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrOutput("connect() Error!");

	while (true)
	{
		cin.getline(massage, BUFSIZE);
		if (sizeof(massage) > BUFSIZE)
			ErrOutput("too many text");
		send(connectSocket, massage, sizeof(massage), 0);
		if (massage == "end")
			break;
	}

	closesocket(connectSocket);
	WSACleanup();
	return 0;
}

void ErrOutput(const char* errMassage)
{
	cout << errMassage << endl;
	exit(1);
}