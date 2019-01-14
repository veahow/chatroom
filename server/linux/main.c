#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

const int BUF_SIZE = 2048;
const int QUEUE_SIZE = 5;
int numLinked = 0;

int sockServ, mainSockCli;
struct sockaddr_in addrServ, addrCli;	// addrServ服务器端 addrCli客户端

int naddr = sizeof(struct sockaddr_in);

int main(int argc, char *argv[])
{
	// 创建socket
	sockServ = socket(AF_INET, SOCK_STREAM, 0);
	
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
			printf("Hello World!\n");
		}
	}


	close(mainSockCli);	// 关闭客户端主套接字
	close(sockServ);	// 关闭服务器端套接字



	return 0;
}

