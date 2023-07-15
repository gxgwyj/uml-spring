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

void main(void)
{
	char *host = "127.0.0.1";
	
	int socketS;
	struct sockaddr_in server_addr;
	
	/*����socket�ļ�������*/ 
	socketS = socket(AF_INET, SOCK_STREAM, 0);
	if(socketS<0)
	{
		/*���������ӷ����쳣ʱ��ֱ��ʹ��perror������ӡ*/ 
		perror("socket");
		return;
	}
	
	/*���÷�������ַ*/
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family =  AF_INET; /*Э����*/
	server_addr.sin_addr.s_addr =  htonl(INADDR_ANY); /*���ص�ַ*/
	server_addr.sin_port =  htons(PORT); /*�������˿�*/
	
	inet_pton(AF_INET,host,&server_addr.sin_addr);
	
	/*���ӷ�����:�������������־ͷ����ڴ˿�*/
	int con = connect(socketS, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)); 
	if(con == -1)
	{
		/*perror �������Դ�ӡ�����ԭ��*/
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
		/*read �����Ĳ���0 ����ӿ���̨��׼��������ȡ*/
		size = read(0,buffer,1024);
		if(size > 0)
		{
		    /*�������ݸ������*/
		    write(s, buffer, size);
		    
		    /*��ȡ����˷��ص�����*/ 
		    size =  read(s,buffer,1024);
		    
		    /*�������ݵ���׼����̨���*/ 
		    write(1, buffer, size);
		}
	
	} 
}
