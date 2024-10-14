#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delayUs(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delayUs(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delayUs(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delayUs(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
      IIC_SDA=(txd&0x80)>>7;
      txd<<=1; 	  
		delayUs(2);   //对IIC来说这三个延时都是必须的
		IIC_SCL=1;
		delayUs(2); 
		IIC_SCL=0;	
		delayUs(2);
    }	 
	 IIC_Wait_Ack();
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	char i,receive=0;
	char bit;
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;   /*这里一定要设成输入上拉，否则不能读出数据*/
//   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
//   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_Init( GPIOB, &GPIO_InitStruct );
	SDA_IN();//SDA设置为输入
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
        IIC_Ack();//发送ACK
    else
        IIC_NAck(); //发送NACK   
//	 		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init( GPIOB, &GPIO_InitStruct );
    return receive;
}



























