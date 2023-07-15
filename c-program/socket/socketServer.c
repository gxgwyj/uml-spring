#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/*�����˿�*/
#define PORT 8086
/*�������г���*/ 
#define BACKLOG 2

void main()
{
	/*����ˡ��ͻ���socket������*/
	int serverC,clientC;
	int res; 
	
	/*����ˡ��ͻ��˵�ַ�ṹ*/
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	
	
	/*����һ����ʽ�׽��� AF_INET����IPv4����Э��  SOCK_STREAM����ʹ��TCP�ɿ�Э��*/
	/*socket���������ں˵��ļ�������*/  
	serverC = socket(AF_INET, SOCK_STREAM, 0);
	if(serverC<0)
	{
		printf("socket error\n");
		return;
	}
	
	/*���÷�������ַ*/
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family =  AF_INET; /*Э����*/
	server_addr.sin_addr.s_addr =  htonl(INADDR_ANY); /*���ص�ַ*/
	server_addr.sin_port =  htons(PORT); /*�������˿ڣ�htons �������ֽ��������ֽ����ת��*/
	
	/*�󶨵�ַ���׽���������*/
	res = bind(serverC, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	if(res<0)
	{
		perror("bind");
		return;
	}
	
	/*���ü�������*/
	res = listen(serverC,BACKLOG);
	if(res<0)
	{
		perror("listen");
		return;
	}
	
	/*����ѭ������ͣ�Ľ��ܿͻ��˵�����Ŀǰ��ʵ��Ϊ˳��ִ�У������ֻ�ܴ���һ���ͻ��˺��ٴ�����һ���ͻ���*/
	for(;;)
	{
		socklen_t addrlen = sizeof(struct sockaddr);
		
		/*���ܿͻ������� accpet�ɹ���᷵��һ���µ��׽����ļ�����������ͻ�������*/ 
		clientC = accept(serverC, (struct sockaddr*)&client_addr, &addrlen);
		
		
		if(clientC<0)
		{
			continue;
		}
	   printf("client connect success \n");
       process_conn_server(clientC);
	}
	
}
/*�������Կͻ��˵Ĵ���*/
void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];
	
	for(;;)
	{
		/*�ӿͻ����ļ��������ж�ȡ����*/ 
		size = read(s,buffer,1024);
		
	    printf("read Client Data \n");
	
		if(size == 0)
		{
			return;
		}
		sprintf(buffer,"%d bytes altogether\n", size);
		
		/*���͸��ͻ���*/
		write(s, buffer, strlen(buffer)+1);
		
	}  
	
}
