#include <stdio.h>

/*定义联合体，用于测试字节序，因为联合体的成员公用一个内存地址，利用该特性判断字节序*/
typedef union{
	unsigned short int value; /*断整型变量*/
	unsigned char byte[2]; /*字符类型*/
}to;


void main()
{
	to typeorder;
	typeorder.value = 0xabcd; /*16进制*/
	
	/*小端字节序检查*/
	if(typeorder.byte[0]==0xcd && typeorder.byte[1]==0xab)
	{
		printf("Low endian byte order byte[0]:0x%x,byte[1]:0x%x\n",typeorder.byte[0],typeorder.byte[1]);
	} 
	
	/*大端字节序检查*/
	if(typeorder.byte[0]==0xab && typeorder.byte[1]==0xcd)
	{
		printf("High endian byte order byte[0]:0x%x,byte[1]:0x%x\n",typeorder.byte[0],typeorder.byte[1]);
	}
} 
 
