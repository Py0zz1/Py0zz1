#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>

int main()
{
	SOCKET socket_c;
	SOCKADDR_IN client_addr;
	WSADATA WSAdata;

	if (WSAStartup(MAKEWORD(2, 2), &WSAdata) != 0)
	{
		printf("���� üũ ���� %d", WSAGetLastError());
		return 0;
	}
	socket_c = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_c == INVALID_SOCKET)
	{
		printf("���� ���� ���� %d", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(1814);

	if (connect(socket_c, (SOCKADDR *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR)
	{
		printf("���� ���� %d", WSAGetLastError());
		closesocket(socket_c);
		WSACleanup();
		return 0;
	}
	printf("���� ����!\n");
	return 0;
}




