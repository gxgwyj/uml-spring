#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/*监听端口*/
#define PORT 8086
/*监听队列长度*/ 
#define BACKLOG 2

void main()
{
	/*服务端、客户端socket描述符*/
	int serverC,clientC;
	int res; 
	
	/*服务端、客户端地址结构*/
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	
	
	/*建立一个流式套接字 AF_INET代表IPv4网络协议  SOCK_STREAM代表使用TCP可靠协议*/
	/*socket函数返回内核的文件描述符*/  
	serverC = socket(AF_INET, SOCK_STREAM, 0);
	if(serverC<0)
	{
		printf("socket error\n");
		return;
	}
	
	/*设置服务器地址*/
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family =  AF_INET; /*协议族*/
	server_addr.sin_addr.s_addr =  htonl(INADDR_ANY); /*本地地址*/
	server_addr.sin_port =  htons(PORT); /*服务器端口，htons 是主机字节序到网络字节序的转换*/
	
	/*绑定地址到套接字描述符*/
	res = bind(serverC, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	if(res<0)
	{
		perror("bind");
		return;
	}
	
	/*设置监听队列*/
	res = listen(serverC,BACKLOG);
	if(res<0)
	{
		perror("listen");
		return;
	}
	
	/*利用循环，不停的接受客户端的请求，目前该实现为顺序执行，服务端只能处理一个客户端后再处理另一个客户端*/
	for(;;)
	{
		socklen_t addrlen = sizeof(struct sockaddr);
		
		/*接受客户端连接 accpet成功后会返回一个新的套接字文件描述符代表客户端连接*/ 
		clientC = accept(serverC, (struct sockaddr*)&client_addr, &addrlen);
		
		
		if(clientC<0)
		{
			continue;
		}
	   printf("client connect success \n");
       process_conn_server(clientC);
	}
	
}
/*服务器对客户端的处理*/
void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];
	
	for(;;)
	{
		/*从客户端文件描述符中读取数据*/ 
		size = read(s,buffer,1024);
		
	    printf("read Client Data \n");
	
		if(size == 0)
		{
			return;
		}
		sprintf(buffer,"%d bytes altogether\n", size);
		
		/*发送给客户端*/
		write(s, buffer, strlen(buffer)+1);
		
	}  
	
}
