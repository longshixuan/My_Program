#include "stm32f10x.h"
#include "sys.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "usart3.h"
#include "iwdg.h"
#include "usart2.h"
#include "dht11.h"
#include "sgp30.h"
#include "led.h"
#include "lsens.h"
#include "TIM2.h"
u8 temp,temp_th=25,huim,open_flag=1;
u32 CO2,sgp30_dat;
u8 open_time=10,stop_time=5,time=0;
float ags;
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    usart_init(115200);
		TIM2_Init();
		DHT11_Init();
		LED_Init();
		Lsens_Init();
		SGP30_Init();
		DelayMs(100);
		SGP30_Write(0x20,0x08);
}

int main(void)
{

    Hardware_Init();

    while(1)
    {
			DHT11_Read_Data(&temp,&huim);
			SGP30_Write(0x20,0x08);
			sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
			CO2 = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
			ags=Lsens_Get_Val_ch4()*53.59+9.2129;
			printf("�¶ȣ�%d��\r\nʪ�ȣ�%d%%\r\n",temp,huim);
			printf("������̼Ũ�ȣ�%d\r\n",(CO2-400));
			printf("����Ũ�ȣ�%.1f\r\n",ags);
			printf("�¶���ֵ��%d��\r\n",temp_th);
			printf("����ʱ�䣺%ds\r\n",open_time);
			printf("ֹͣʱ�䣺%ds\r\n\r\n\r\n",stop_time);
			temp<=temp_th?LED1_ON:LED1_OFF;			//���¼���
			if(open_time==0)
			{
				LED2_OFF;
				time=0;
			}
			else if(stop_time==0)
			{
				LED2_ON;
				time=0;
			}
			else if(open_flag==1)
			{
				if(time>=open_time)
				{
					LED2_OFF;
					time=0;
					open_flag=0;
				}
				else
				{
					LED2_ON;
				}
			}
			else if(open_flag==0)
			{
				if(time>=stop_time)
				{
					LED2_ON;
					time=0;
					open_flag=1;
				}
				else
				{
				
					LED2_OFF;
					
				}
			}
			time++;
			Delay_ms(850);
			
    }
}

