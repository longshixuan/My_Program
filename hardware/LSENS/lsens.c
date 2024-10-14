#include "lsens.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//LSENS(����������)��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ������������
void Lsens_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4;//PA1 PA4 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	Adc1_Init();
}
//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 
float Lsens_Get_Val_ch1(void)
{
	float temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc1_chx(LSENS_ADC1_CH1);	//��ȡADCֵ
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	temp_val=temp_val*3.3/4095;
	return (float)temp_val;
}


float Lsens_Get_Val_ch4(void)
{
	float temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc1_chx(LSENS_ADC1_CH4);	//��ȡADCֵ
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	temp_val=temp_val*3.3/4095;
	return (float)temp_val;
}

//��ȡLight Sens��ֵ
//0~100:0,�;100,���� 


/*
*********************************��ȩ*********************************************
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
		temp_val+=Get_Adc1_chx(LSENS_ADC1_CH4);	//��ȡADCֵ
		DelayMs(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	temp_val=temp_val*3.3/4096.00;

}
*/

