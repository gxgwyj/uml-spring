#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fd = -1; /*�ļ�����������*/
	char filename[] = "test.txt";
	fd = open(filename,O_RDWR); /*�����ں˺������ļ������ҷ����ļ�������*/ 
	if(fd==-1){
		printf("Open file %s failure!,fd:%d\n",filename,fd);
	}else{
		printf("Open file %s success!,fd:%d\n",filename,fd);
	}
	return 0;
}
