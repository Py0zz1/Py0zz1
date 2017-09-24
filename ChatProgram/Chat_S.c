#pragma comment(lib,"ws2_32.lib")
//#define WIN32_LEAN_AND_MEAN
#define BUFSIZE 1024
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>
#include <conio.h>


int main()
{

	SOCKET socket_S, socket_C;
	SOCKADDR_IN server_addr,client_addr;
	WSADATA WSAdata;
	int str_size;
	int nTimeOut = 200;
	char msg[BUFSIZE];
	char data[BUFSIZE];
	char ID[30];

	if (WSAStartup(MAKEWORD(2, 2), &WSAdata) != 0)
	{
		printf("버젼체크 오류 %d\n", WSAGetLastError());
		return 0;
	}


	socket_S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_S == INVALID_SOCKET)
	{
		printf("소켓 생성 오류 %d\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1814);
	server_addr.sin_addr.s_addr = INADDR_ANY;


	if (bind(socket_S, (SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		printf("bind 에러 %d\n\n", WSAGetLastError());
		closesocket(socket_S);
		WSACleanup();
		return 0;
	}



	if (listen(socket_S, 5) != 0)
	{
		printf("listen 에러 %d\n", WSAGetLastError());
		closesocket(socket_S);
		WSACleanup();
		return 0;
	}

	printf("\t\t\t\t\t접속대기중...\n");

	int size = sizeof(client_addr);
	socket_C = accept(socket_S, (SOCKADDR *)&client_addr, &size);
	int size2 = sizeof(int);
	setsockopt(socket_C, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeOut, size2);
	printf("My Char Log");
	printf("\t\t\t\t\t연결 성공\n");
printf("\t\t\t\t\tID : ");
gets(ID);
	puts("\t\t\t\t\t=====채팅시작=====");
	while (1)
	{
		if (kbhit())
		{
			gets(data);
			if (strcmp(data, "exit") ==0)
			{
				break;
			}
			sprintf(msg,"\t\t\t\t\t%s : %s\n", ID, data);
			if (send(socket_C, msg, sizeof(msg), 0) < sizeof(msg))
			{
				printf("%d", WSAGetLastError());
				return 0;
			}
		}
		memset(msg, 0,sizeof data);
		str_size = recv(socket_C, msg, BUFSIZE - 1, 0);
		if (str_size == 0 || WSAGetLastError() == WSAETIMEDOUT) continue;
		msg[str_size] = 0;
		if (strcmp(msg, "exit") == 0)
		{
			printf("\t\t\t\t\tClient가 접속을 종료했습니다.\n");
		}
		else printf("%s", msg);
		
	}
	
	if (closesocket(socket_S) != 0 || closesocket(socket_C) != 0)
	{
		printf("비정상 종료 %d\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	if (WSACleanup() != 0)
	{
		printf("WS2_32.DLL 종료 에러 %d", WSAGetLastError());
		return 0;
	}
}