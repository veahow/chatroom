// client.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")

const int BUF_SIZE = 2048;    // 缓冲区

SOCKET sockServ, sockCli;    // 服务器端sockServ 客户端sockCli
SOCKADDR_IN addrServ, addrCli;    // 服务器端地址addrServ 客户端地址addrCli
int queuesize = 5;
int naddr = sizeof(SOCKADDR_IN);

char inputBuf[BUF_SIZE];    // 输入字符数组缓冲区

char username[BUF_SIZE];
char password[BUF_SIZE];

char ip[128];

DWORD WINAPI TransmitInfo(LPVOID lpParam);
DWORD WINAPI RecvInfo(LPVOID lpParam);

void getIP()
{
	char host_name[128];
	gethostname(host_name, sizeof(host_name));

	hostent *lv_pHostent;
	lv_pHostent = (hostent *)malloc(sizeof(hostent));
	if (NULL == (lv_pHostent = gethostbyname(host_name)))
	{
		printf("get Hostname Fail \n");
		return ;
	}
	SOCKADDR_IN lv_sa;
	lv_sa.sin_family = AF_INET;
	lv_sa.sin_port = htons(6000);
	memcpy(&lv_sa.sin_addr.S_un.S_addr,
		lv_pHostent->h_addr_list[0], lv_pHostent->h_length);
	strcpy(ip, inet_ntoa(lv_sa.sin_addr));
	return;
}

int main()
{
	// 首先要加载socket库
	WSADATA wsaData;
	WORD wVersionRequested;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err) {
		std::cout << "载入socket库失败！按任意键退出程序" << std::endl;

		getchar();
		return 0;
	}

	// 创建socket
	sockCli = socket(AF_INET, SOCK_STREAM, 0);

	// 初始化客户端的端点地址（IP地址+Port端口号）
	addrCli.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");    // 本地ip测试
	addrCli.sin_family = AF_INET;
	addrCli.sin_port = htons(8080);

	// 初始化服务器端的端点地址（IP地址+Port端口号）
	getIP();
	addrServ.sin_addr.S_un.S_addr = inet_addr(ip);
	//addrServ.sin_addr.S_un.S_addr = inet_addr("172.24.34.74");

	std::cout << "Your IP Address: " << ip << std::endl;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(8080);

	for (;;) {
		if (connect(sockCli, (SOCKADDR*)&addrServ, sizeof(addrServ)) != SOCKET_ERROR) {

			std::cout << "成功连接服务器..." << std::endl;
			send(sockCli, ip, BUF_SIZE, 0);
			
			
			::CreateThread(nullptr, 0, TransmitInfo, (LPVOID)sockCli, 0, nullptr);
			::CreateThread(nullptr, 0, RecvInfo, (LPVOID)sockCli, 0, nullptr);
		
		}
	}
	
	closesocket(sockCli);
	closesocket(sockServ);
	
	WSACleanup();

    return 0;
}

DWORD WINAPI TransmitInfo(LPVOID lpParam)
{
	SOCKET sockCli = (SOCKET)lpParam;
	char sendBuf[BUF_SIZE];    // 发送字符数组缓冲区
	for (;;) {
		memset(sendBuf, 0, sizeof(sendBuf));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_GREEN |
			FOREGROUND_BLUE |
			FOREGROUND_RED |
			FOREGROUND_INTENSITY);
		
		// gets会缓冲区溢出
		fgets(sendBuf, BUF_SIZE, stdin);
		sendBuf[strlen(sendBuf)-1] = '\0';// 去除尾端换行符
		if(send(sockCli, sendBuf, BUF_SIZE, 0) <= 0) break;
	}
	closesocket(sockCli);
	return 0;
}

DWORD WINAPI RecvInfo(LPVOID lpParam)
{
	SOCKET sockCli = (SOCKET)lpParam;
	char recvBuf[BUF_SIZE];    // 接收字符数组缓冲区
	for (;;) {
		memset(recvBuf, 0, sizeof(recvBuf));
		if(recv(sockCli, recvBuf, BUF_SIZE, 0) <= 0) break;    // 失败或退出
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		
		std::cout << recvBuf;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_GREEN |
			FOREGROUND_BLUE |
			FOREGROUND_RED |
			FOREGROUND_INTENSITY);
	}
	closesocket(sockCli);

	return 0;
}
