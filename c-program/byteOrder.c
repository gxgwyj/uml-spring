#include <stdio.h>
#include <arpa/inet.h>

#define BITS16 16
#define BITS32 32


/*字节序转换例子*/

/*16位字节序转换联合体*/
typedef union
{
	unsigned short int value; /*16位的短整型变量*/
	unsigned char byte[2]; /*2位char类型的数组*/
}to16; 


/*32位字节序转换结构体*/
typedef union
{
	unsigned long int value; /*32位的长整型变量*/
	unsigned char byte[4]; /*4位char类型的数组*/
}to32;

/**
begin 表示变量的地址指针
flag  表示字长 
*/
void showValue(unsigned char *begin,int flag)
{
	int num = 0,i = 0;
	if(flag == BITS16)
	{
		num = 2;
	}else if(flag == BITS32)
	{
		num = 4;
	}
	for(i=0;i<num;i++){
		printf("%x ",*(begin+i));
	}
	printf("\n"); 
}

void main()
{
	to16 v16_orig,v16_turn1,v16_turn2;
	to32 v32_orig,v32_turn1,v32_turn2;
	
	/*16位转换*/ 
	v16_orig.value = 0xabcd;/*赋值为0xabcd*/
	v16_turn1.value = htons(v16_orig.value);/*第1次转换*/
	v16_turn2.value = htons(v16_turn1.value);/*第2次转换*/
	
	
	/*32位转换*/ 
	v32_orig.value = 0x12345678;/*赋值为0x12345678*/
	v32_turn1.value = htonl(v32_orig.value);/*第1次转换*/
	v32_turn2.value = htonl(v32_turn1.value);/*第2次转换*/
	
	/*打印结果*/ 
	printf("16 host to network byte order change:\n");
	printf("\t orig:\t");
	showValue(v16_orig.byte,BITS16);
	printf("\t 1 times:");
	showValue(v16_turn1.byte,BITS16);
	printf("\t 2 times:");
	showValue(v16_turn2.byte,BITS16);
	
	printf("32 host to network byte order change:\n");
	printf("\t orig:\t");
	showValue(v32_orig.byte,BITS32);
	printf("\t 1 times:");
	showValue(v32_turn1.byte,BITS32);
	printf("\t 2 times:");
	showValue(v32_turn2.byte,BITS32);
	
}
