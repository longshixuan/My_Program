#ifndef __BH1750_H
#define __BH1750_H
#include "myiic.h"   
		  
extern unsigned char  data[10];
#define	  Address   0x46 ////����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //  ADDRESS���Žӵ�ʱ��ַΪ0x46
void BH1750_WriteOneByte(u16 SlaveAddress,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void BH1750_WriteLenByte(u16 SlaveAddress,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
void BH1750_Write(u16 SlaveAddress,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void BH1750_Read(void);   	//��ָ����ַ��ʼ����ָ�����ȵ�����
void BH1750_Init(void); //��ʼ��IIC
#endif
















