#include "bh1750.h" 
#include "delay.h"

//��ʼ��IIC�ӿ�
void BH1750_Init(void)
{
	IIC_Init();
}

//��ָ����ַд��һ������
//SlaveAddress  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void BH1750_WriteOneByte(u16 SlaveAddress,u8 DataToWrite)
{				   	  	    																 
	 IIC_Start();
	DelayMs(5);
	IIC_Send_Byte(SlaveAddress);//�����豸��ַ+д�ź�
	DelayMs(5);
	IIC_Send_Byte(DataToWrite); //�ڲ��Ĵ�����ַ
	DelayMs(5);
	IIC_Stop();
}



//��������BH1750�ڲ�����
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











