#ifndef __BH1750_H
#define __BH1750_H
#include "myiic.h"   
		  
extern unsigned char  data[10];
#define	  Address   0x46 ////定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //  ADDRESS引脚接地时地址为0x46
void BH1750_WriteOneByte(u16 SlaveAddress,u8 DataToWrite);		//指定地址写入一个字节
void BH1750_WriteLenByte(u16 SlaveAddress,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
void BH1750_Write(u16 SlaveAddress,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void BH1750_Read(void);   	//从指定地址开始读出指定长度的数据
void BH1750_Init(void); //初始化IIC
#endif
















