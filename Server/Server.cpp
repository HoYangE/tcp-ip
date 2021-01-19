#include <iostream>
#include <winsock2.h>
#include <list>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 512

using namespace std;

void ErrOutput(const char *errMassage);
DWORD WINAPI SendMassage(LPVOID connectSocket);

int main()
{
	WSADATA wsaData;
	SOCKET listenSocket, connectSocket;
	SOCKADDR_IN serverAddr, clientAddr;

	int sizeofClientAddr;
	//list<SOCKET> connectSocketList;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrOutput("WASStartup() Error!");

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
		ErrOutput("socket() Error!");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(12345);

	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrOutput("bind() Error!");

	if (listen(listenSocket, 5) == SOCKET_ERROR)
		ErrOutput("listen() Error!");

	cout << "Waitting Client" << endl;

	HANDLE hThread;

	while (true)
	{
		sizeofClientAddr = sizeof(clientAddr);
		connectSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &sizeofClientAddr);
		if (connectSocket == INVALID_SOCKET)
			ErrOutput("accept() Error!");

		//connectSocketList.push_back(connectSocket);
		cout << "ConnectClient : " << connectSocket << endl;

		hThread = CreateThread(NULL, 0, SendMassage, (LPVOID)connectSocket, 0, NULL);

		if (hThread == NULL)
			closesocket(connectSocket);
		else
			CloseHandle(hThread);
	}

	//closesocket(connectSocket);
	closesocket(listenSocket);
	WSACleanup();
	return 0;
}

void ErrOutput(const char* errMassage)
{
	cout << errMassage << endl;
	exit(1);
}

DWORD WINAPI SendMassage(LPVOID connectSocket)
{
	char massage[BUFSIZE];
	while (true)
	{
		int recvSize = recv((SOCKET)connectSocket, massage, sizeof(massage), 0);
		if (recvSize == -1)
		{
			//connectSocketList.remove(*connectSocket);
			ErrOutput("recv() Error!");
		}
		if (massage == "end")
		{
			//connectSocketList.remove(*connectSocket);
			break;
		}
		cout << "Socket : " << (SOCKET)connectSocket << " -> " << massage << endl;
	}
	cout << "Close Socket : " << (SOCKET)connectSocket << endl;
	closesocket((SOCKET)connectSocket);
	return 0;
}