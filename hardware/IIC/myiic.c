#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delayUs(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delayUs(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delayUs(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delayUs(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delayUs(1);	   
	IIC_SCL=1;delayUs(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delayUs(2);
	IIC_SCL=1;
	delayUs(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delayUs(2);
	IIC_SCL=1;
	delayUs(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
      IIC_SDA=(txd&0x80)>>7;
      txd<<=1; 	  
		delayUs(2);   //��IIC��˵��������ʱ���Ǳ����
		IIC_SCL=1;
		delayUs(2); 
		IIC_SCL=0;	
		delayUs(2);
    }	 
	 IIC_Wait_Ack();
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	char i,receive=0;
	char bit;
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;   /*����һ��Ҫ������������������ܶ�������*/
//   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
//   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init( GPIOB, &GPIO_InitStruct );
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{		
		receive<<=1;
//        IIC_SCL=0; 
//        delayUs(2);
		IIC_SCL=1;
				delayUs(2);
        if(READ_SDA)bit=0x01;
		
				else bit-0x00;
		delayUs(1);
				receive|=bit;
		IIC_SCL=0;
    }					 
    if (!ack)
        IIC_Ack();//����ACK
    else
        IIC_NAck(); //����NACK   
//	 		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init( GPIOB, &GPIO_InitStruct );
    return receive;
}



























