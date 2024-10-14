#ifndef __LSENS_H
#define __LSENS_H	
#include "sys.h" 
#include "adc.h" 
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
  
#define LSENS_READ_TIMES	10		//���������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
#define LSENS_ADC1_CH1		ADC_Channel_1	//����������������ڵ�ADC1ͨ��4���
#define LSENS_ADC1_CH4		ADC_Channel_4	//����������������ڵ�ADC1ͨ��4���

    
void Lsens_Init(void); 				//��ʼ������������
float Lsens_Get_Val_ch1(void);
float Lsens_Get_Val_ch4(void);
#endif 





















