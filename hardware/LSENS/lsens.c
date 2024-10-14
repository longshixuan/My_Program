#include "lsens.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//LSENS(光敏传感器)驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
 
//初始化光敏传感器
void Lsens_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4;//PA1 PA4 anolog输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	Adc1_Init();
}
//读取Light Sens的值
//0~100:0,最暗;100,最亮 
float Lsens_Get_Val_ch1(void)
{
	float temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc1_chx(LSENS_ADC1_CH1);	//读取ADC值
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//得到平均值 
	temp_val=temp_val*3.3/4095;
	return (float)temp_val;
}


float Lsens_Get_Val_ch4(void)
{
	float temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc1_chx(LSENS_ADC1_CH4);	//读取ADC值
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//得到平均值 
	temp_val=temp_val*3.3/4095;
	return (float)temp_val;
}

//读取Light Sens的值
//0~100:0,最暗;100,最亮 


/*
*********************************甲醛*********************************************
Formaldehyde=Lsens_Get_Val_ch4();
			Formaldehyde=(-0.0243*Formaldehyde*Formaldehyde) + (0.4925*Formaldehyde) + 0.7911;
			sprintf(buffer,"%.2f",Formaldehyde);
			OLED_ShowString(16*3,6,buffer,16);
float Lsens_Get_Val_ch4(void)
{

	float temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc1_chx(LSENS_ADC1_CH4);	//读取ADC值
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//得到平均值 
	temp_val=temp_val*3.3/4096.00;

}
*/

