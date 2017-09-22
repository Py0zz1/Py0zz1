#pragma comment(lib,"ws2_32.lib")
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>


int main()
{

	SOCKET socket_S, socket_C;
	SOCKADDR_IN server_addr,client_addr;
	WSADATA WSAdata;
	


	if (WSAStartup(MAKEWORD(2, 2), &WSAdata) != 0)
	{
		printf("����üũ ���� %d\n", WSAGetLastError());
		return 0;
	}


	socket_S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_S == INVALID_SOCKET)
	{
		printf("���� ���� ���� %d\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1814);
	server_addr.sin_addr.s_addr = INADDR_ANY;


	if (bind(socket_S, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		printf("bind ���� %d\n\n", WSAGetLastError());
		closesocket(socket_S);
		WSACleanup();
		return 0;
	}



	if (listen(socket_S, 5) != 0)
	{
		printf("listen ���� %d\n", WSAGetLastError());
		closesocket(socket_S);
		WSACleanup();
		return 0;
	}

	printf("accept : ���� ��û ��� �� \n");

	int size = sizeof(client_addr);
	socket_C = accept(socket_S, (SOCKADDR *)&client_addr, &size);

	printf("���� ����\n");



	if (closesocket(socket_S) != 0 || closesocket(socket_C) != 0)
	{
		printf("������ ���� %d\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	if (WSACleanup() != 0)
	{
		printf("WS2_32.DLL ���� ���� %d", WSAGetLastError());
		return 0;
	}
}