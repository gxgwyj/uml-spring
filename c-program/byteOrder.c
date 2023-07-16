#include <stdio.h>
#include <arpa/inet.h>

#define BITS16 16
#define BITS32 32


/*�ֽ���ת������*/

/*16λ�ֽ���ת��������*/
typedef union
{
	unsigned short int value; /*16λ�Ķ����ͱ���*/
	unsigned char byte[2]; /*2λchar���͵�����*/
}to16; 


/*32λ�ֽ���ת���ṹ��*/
typedef union
{
	unsigned long int value; /*32λ�ĳ����ͱ���*/
	unsigned char byte[4]; /*4λchar���͵�����*/
}to32;

/**
begin ��ʾ�����ĵ�ַָ��
flag  ��ʾ�ֳ� 
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
	
	/*16λת��*/ 
	v16_orig.value = 0xabcd;/*��ֵΪ0xabcd*/
	v16_turn1.value = htons(v16_orig.value);/*��1��ת��*/
	v16_turn2.value = htons(v16_turn1.value);/*��2��ת��*/
	
	
	/*32λת��*/ 
	v32_orig.value = 0x12345678;/*��ֵΪ0x12345678*/
	v32_turn1.value = htonl(v32_orig.value);/*��1��ת��*/
	v32_turn2.value = htonl(v32_turn1.value);/*��2��ת��*/
	
	/*��ӡ���*/ 
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
