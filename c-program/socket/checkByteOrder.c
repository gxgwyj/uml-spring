#include <stdio.h>

/*���������壬���ڲ����ֽ�����Ϊ������ĳ�Ա����һ���ڴ��ַ�����ø������ж��ֽ���*/
typedef union{
	unsigned short int value; /*�����ͱ���*/
	unsigned char byte[2]; /*�ַ�����*/
}to;


void main()
{
	to typeorder;
	typeorder.value = 0xabcd; /*16����*/
	
	/*С���ֽ�����*/
	if(typeorder.byte[0]==0xcd && typeorder.byte[1]==0xab)
	{
		printf("Low endian byte order byte[0]:0x%x,byte[1]:0x%x\n",typeorder.byte[0],typeorder.byte[1]);
	} 
	
	/*����ֽ�����*/
	if(typeorder.byte[0]==0xab && typeorder.byte[1]==0xcd)
	{
		printf("High endian byte order byte[0]:0x%x,byte[1]:0x%x\n",typeorder.byte[0],typeorder.byte[1]);
	}
} 
 
