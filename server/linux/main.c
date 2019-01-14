#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 2048
#define QUEUE_SIZE 5
int numLinked = 0;

int sockServ, mainSockCli;
struct sockaddr_in addrServ, addrCli;	// addrServ服务器端 addrCli客户端
int socks[QUEUE_SIZE];

int naddr = sizeof(struct sockaddr_in);

void* CreateClientThread(void* args)
{
	int sockCli = *(int *)args;

	char ip[BUF_SIZE];
	char recvBuf[BUF_SIZE];
	char buf[BUF_SIZE];

	memset(ip, 0 , sizeof(ip));
	memset(recvBuf, 0, sizeof(recvBuf));
	memset(buf, 0, sizeof(buf));

	recv(sockCli, ip, BUF_SIZE, 0);
	printf("%s 连接服务器成功\n", ip);

	socks[numLinked++] = sockCli;

	for(;;){
		memset(buf, 0, sizeof(buf));
		memset(recvBuf, 0, sizeof(recvBuf));

		if(recv(sockCli, recvBuf, BUF_SIZE, 0) <= 0 ){
			printf("%s 退出服务器\n", ip);
			break;
		}

		printf("[%s]:%s\n", ip, recvBuf);
		snprintf(buf, sizeof(buf), "[%s]:%s\n", ip, recvBuf);
		for(int i = 0; i < numLinked; ++i){
			send(socks[i], buf, BUF_SIZE, 0);
		}
	}

	close(sockCli);

	return NULL;
}

int main(int argc, char *argv[])
{
	// 创建socket
	sockServ = socket(AF_INET, SOCK_STREAM, 0);
	pthread_t th;
	
    //	addrServ.sin_addr.s_addr = htonl(INADDR_ANY);
	addrServ.sin_addr.s_addr = inet_addr("192.168.43.188");
	addrServ.sin_family = AF_INET;	// TCP/IP IPv4
	addrServ.sin_port = htons(8080);	// 8080端口

	bind(sockServ, (struct sockaddr*)&addrServ, sizeof(addrServ));	// 绑定socket
	listen(sockServ, QUEUE_SIZE);	// 监听socket

	for(;;){
	
		// 主套接字接受连接请求
		mainSockCli = accept(sockServ, (struct sockaddr*)&addrCli, &naddr);

		if(mainSockCli != -1){
			pthread_create(&th, NULL, CreateClientThread, &mainSockCli);
		}
	}


	close(mainSockCli);	// 关闭客户端主套接字
	close(sockServ);	// 关闭服务器端套接字



	return 0;
}

