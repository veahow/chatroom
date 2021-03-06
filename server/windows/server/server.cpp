// server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")

const int BUF_SIZE = 2048;    // 缓冲区
const int queuesize = 5;    // 最多连5个
int numLinked = 0;    // 连接数

SOCKET sockServ, mainSockCli;    // 服务器端sockServ 客户端sockCli
SOCKADDR_IN addrServ, addrCli;    // 服务器端地址addrServ 客户端地址addrCli
SOCKET sock[queuesize];    // 这里其实可以用队列

int naddr = sizeof(SOCKADDR_IN);

char inputBuf[BUF_SIZE];    // 输入字符数组缓冲区
char sendBuf[BUF_SIZE];    // 发送字符数组缓冲区

char username[BUF_SIZE];
char password[BUF_SIZE];

char ip[128];

DWORD WINAPI CreateClientThread(LPVOID lpParam);

void Handle()
{
	// 连接成功
	std::cout << "客户端连接成功！" << std::endl;

	recv(mainSockCli, username, BUF_SIZE, 0);
	recv(mainSockCli, password, BUF_SIZE, 0);

	User usr(username, password);
	DataBase db("socket", "socket", "localhost", "helloa", 3306);
	if (db.Connect()) {
		if (db.Login(usr))    // 登陆成功返回true
		{
			printf("%s 登陆成功！\n", usr.getUsername());
			strcpy(sendBuf, "Login Successful");
			send(mainSockCli, sendBuf, BUF_SIZE, 0);
		}
		else {
			printf("%s 登录失败！\n", usr.getUsername());
		}
		// db.Register(usr1);    // 注册成功返回true
	}
	else
	{
		printf("Error\n");
	}

	// 清空登录

	
	
}

void getIP()
{
	char host_name[128];
	gethostname(host_name, sizeof(host_name));

	hostent *lv_pHostent;
	lv_pHostent = (hostent *)malloc(sizeof(hostent));
	if (NULL == (lv_pHostent = gethostbyname(host_name)))
	{
		printf("get Hostname Fail \n");
		return;
	}
	SOCKADDR_IN lv_sa;
	lv_sa.sin_family = AF_INET;
	lv_sa.sin_port = htons(6000);
	memcpy(&lv_sa.sin_addr.S_un.S_addr,
		lv_pHostent->h_addr_list[0], lv_pHostent->h_length);
	strcpy(ip, inet_ntoa(lv_sa.sin_addr));
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
	sockServ = socket(AF_INET, SOCK_STREAM, 0);

	// 初始化服务器端的端点地址（IP地址+Port端口号）
	getIP();
	addrServ.sin_addr.S_un.S_addr = inet_addr(ip);    // 实验室网络 把本地电脑作为服务器ip
	//addrServ.sin_addr.S_un.S_addr = inet_addr("172.24.34.74");    // 实验室网络 把本地电脑作为服务器ip
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(8080);    // 端口号8080

	// 绑定socket
	bind(sockServ, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	listen(sockServ, queuesize);
	// 循环监听 可以进行多线程编程改进
	for (;;) {
		// 监听连接请求
		//listen(sockServ, queuesize);

		// 接受连接请求
		mainSockCli = accept(sockServ, (SOCKADDR *)&addrCli, &naddr);    // 放在创建的客户端套接字中
		
		if (mainSockCli != INVALID_SOCKET) {
			::CreateThread(nullptr, 0, CreateClientThread, (LPVOID)mainSockCli, 0, nullptr);
			//std::cout << "hello" << std::endl;
		}

	}

	closesocket(mainSockCli);    // 关闭客户端主套接字
	closesocket(sockServ);    // 关闭服务器端套接字
	WSACleanup(); 
	
    return 0;
}

DWORD WINAPI CreateClientThread(LPVOID lpParam)
{
	SOCKET sockCli = (SOCKET)lpParam;
	char ip[BUF_SIZE];
	char recvBuf[BUF_SIZE];    // 接收字符数组缓冲区
	char buf[BUF_SIZE];

	memset(ip, 0, sizeof(ip));
	memset(recvBuf, 0, sizeof(recvBuf));
	memset(buf, 0, sizeof(buf));

	recv(sockCli, ip, BUF_SIZE, 0);
	//ip[strlen(ip) - 1] = '\0';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
		FOREGROUND_BLUE | 
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);
	std::cout << ip << " 连接服务器成功" << std::endl;

	sock[numLinked++] = sockCli;

	//::CreateThread(nullptr, 0, TransmitInfo, (LPVOID)sockCli, 0, nullptr);
	for (;;) {
		memset(buf, 0, sizeof(buf));
		memset(recvBuf, 0, sizeof(recvBuf));
		
		if (recv(sockCli, recvBuf, BUF_SIZE, 0) <= 0) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << ip << " 退出服务器" << std::endl;
			break;
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		
		std::cout << "[" << ip << "]:" << recvBuf << std::endl;
		sprintf_s(buf, "[%s]:%s\n", ip, recvBuf);
		for (int i = 0; i < numLinked; ++i) {
			//if (sock[i] != sockCli) send(sock[i], buf, BUF_SIZE, 0);
			send(sock[i], buf, BUF_SIZE, 0);
		}
		
	}
	
	closesocket(sockCli);

	return 0;
}

