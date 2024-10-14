#include "bh1750.h" 
#include "delay.h"

//初始化IIC接口
void BH1750_Init(void)
{
	IIC_Init();
}

//在指定地址写入一个数据
//SlaveAddress  :写入数据的目的地址    
//DataToWrite:要写入的数据
void BH1750_WriteOneByte(u16 SlaveAddress,u8 DataToWrite)
{				   	  	    																 
	 IIC_Start();
	DelayMs(5);
	IIC_Send_Byte(SlaveAddress);//发送设备地址+写信号
	DelayMs(5);
	IIC_Send_Byte(DataToWrite); //内部寄存器地址
	DelayMs(5);
	IIC_Stop();
}



//连续读出BH1750内部数据
void BH1750_Read(void)
{
	u8 i;
	IIC_Start();
	IIC_Send_Byte(Address+1);
	for(i=0;i<3;i++)
	{
		
		data[i]=IIC_Read_Byte(0);
		if(i==3)
		{
			IIC_NAck();
		}
		else
		{
			IIC_Ack();
		}
	}
	IIC_Stop();
	DelayMs(3);
	
}  











