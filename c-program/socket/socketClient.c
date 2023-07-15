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

void main(void)
{
	char *host = "127.0.0.1";
	
	int socketS;
	struct sockaddr_in server_addr;
	
	/*创建socket文件描述符*/ 
	socketS = socket(AF_INET, SOCK_STREAM, 0);
	if(socketS<0)
	{
		/*当创建连接发生异常时，直接使用perror函数打印*/ 
		perror("socket");
		return;
	}
	
	/*设置服务器地址*/
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family =  AF_INET; /*协议族*/
	server_addr.sin_addr.s_addr =  htonl(INADDR_ANY); /*本地地址*/
	server_addr.sin_port =  htons(PORT); /*服务器端口*/
	
	inet_pton(AF_INET,host,&server_addr.sin_addr);
	
	/*连接服务器:著名的三次握手就发生在此刻*/
	int con = connect(socketS, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)); 
	if(con == -1)
	{
		/*perror 函数可以打印具体的原因*/
		perror("connect");
		return; 
	}
	
	process_conn_client(socketS);
	
	close(socketS);
	
}

void process_conn_client(int s)
{	
	for(;;)
	{
		ssize_t size = 0;
	    char buffer[1024];
		/*read 函数的参数0 代表从控制台标准输入流读取*/
		size = read(0,buffer,1024);
		if(size > 0)
		{
		    /*发送数据给服务端*/
		    write(s, buffer, size);
		    
		    /*读取服务端返回的数据*/ 
		    size =  read(s,buffer,1024);
		    
		    /*发送数据到标准控制台输出*/ 
		    write(1, buffer, size);
		}
	
	} 
}
