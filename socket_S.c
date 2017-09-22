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

	printf("accept : 연결 요청 대기 중 \n");

	int size = sizeof(client_addr);
	socket_C = accept(socket_S, (SOCKADDR *)&client_addr, &size);

	printf("연결 성공\n");



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