// server.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")

const int BUF_SIZE = 2048;    // 缓冲区

SOCKET sockServ, sockCli;    // 服务器端sockServ 客户端sockCli
SOCKADDR_IN addrServ, addrCli;    // 服务器端地址addrServ 客户端地址addrCli
int queuesize = 5;
int naddr = sizeof(SOCKADDR_IN);

char inputBuf[BUF_SIZE];    // 输入字符数组缓冲区
char sendBuf[BUF_SIZE];    // 发送字符数组缓冲区
char recvBuf[BUF_SIZE];    // 接收字符数组缓冲区

char username[BUF_SIZE];
char password[BUF_SIZE];

void Handle()
{
	// 连接成功
	std::cout << "客户端连接成功！" << std::endl;

	recv(sockCli, username, BUF_SIZE, 0);
	recv(sockCli, password, BUF_SIZE, 0);

	User usr(username, password);
	DataBase db("socket", "socket", "localhost", "helloa", 3306);
	if (db.Connect()) {
		if (db.Login(usr))    // 登陆成功返回true
		{
			printf("%s 登陆成功！\n", usr.getUsername());
			strcpy(sendBuf, "Login Successful");
			send(sockCli, sendBuf, BUF_SIZE, 0);
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
	//addrServ.sin_addr.S_un.S_addr = inet_addr("172.26.128.16");    // 把本地电脑作为服务器ip
	addrServ.sin_addr.S_un.S_addr = inet_addr("192.168.43.188");    // 把本地电脑作为服务器ip 连接手机热点构建局域网测试安卓app
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
		sockCli = accept(sockServ, (SOCKADDR *)&addrCli, &naddr);    // 放在创建的客户端套接字中
		
		if (sockCli != INVALID_SOCKET) {
			
			std::cout << "客户端与服务器连接成功!" << std::endl;
			/*strcpy(sendBuf, "hello啊");

			send(sockCli, sendBuf, BUF_SIZE, 0);*/
			recv(sockCli, recvBuf, BUF_SIZE, 0);
			std::cout << recvBuf << std::endl;

			
			// Handle();

			//closesocket(sockCli);

		}

	}
	closesocket(sockCli);

	//User usr("aaa", "aaa");
	//User usr1("cyh", "cyh");
	//DataBase db("socket", "socket", "localhost", "helloa", 3306);
	//if (db.Connect()) {
	//	// db.Login(usr);    // 登陆成功返回true
	//	// db.Register(usr1);    // 注册成功返回true
	//}
	//else {
	//	printf("Error\n");
	//}

	closesocket(sockServ);
	WSACleanup();

	
    return 0;
}

