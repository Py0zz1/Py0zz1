#pragma comment(lib,"ws2_32.lib")
#define BUFSIZE 1024
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <conio.h>

int main()
{
	SOCKET socket_c;
	SOCKADDR_IN client_addr;
	WSADATA WSAdata;
	char msg[BUFSIZE];
	char data[BUFSIZE];
	char ID[30];
	int str_size;
	int nTimeOut = 200;
	if (WSAStartup(MAKEWORD(2, 2), &WSAdata) != 0)
	{
		printf("버젼 체크 오류 %d", WSAGetLastError());
		return 0;
	}
	socket_c = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_c == INVALID_SOCKET)
	{
		printf("소켓 생성 오류 %d", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(1814);

	if (connect(socket_c, (SOCKADDR *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR)
	{
		printf("연결 오류 %d", WSAGetLastError());
		closesocket(socket_c);
		WSACleanup();
		return 0;
	}
	printf("My Chat Log");
	printf("\t\t\t\t\t연결 성공!\n");
	
printf("\t\t\t\t\tID : ");
gets(ID);
	printf("\t\t\t\t\t=====채팅시작=====\n");
	
	int size = sizeof(int);
	setsockopt(socket_c, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeOut, size);
	
	while (1)
	{
		if (kbhit())
		{
			gets(data);
			if (strcmp(data, "exit") == 0)
			{
				send(socket_c, data, sizeof(data), 0);
				break;
			}
			sprintf(msg,"\t\t\t\t\t%s : %s\n", ID, data);
			send(socket_c, msg, sizeof(msg), 0);

		}
		memset(msg, 0, sizeof data);
		str_size = recv(socket_c, msg, BUFSIZE - 1, 0);
		if (str_size == 0 || WSAGetLastError() == WSAETIMEDOUT) continue;
		msg[str_size] = 0;
		printf("%s", msg);

	}
	closesocket(socket_c);
	return 0;
}




