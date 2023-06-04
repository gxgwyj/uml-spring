#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fd = -1; /*文件描述符申明*/
	char filename[] = "test.txt";
	fd = open(filename,O_RDWR); /*调用内核函数打开文件，并且返回文件描述符*/ 
	if(fd==-1){
		printf("Open file %s failure!,fd:%d\n",filename,fd);
	}else{
		printf("Open file %s success!,fd:%d\n",filename,fd);
	}
	return 0;
}
