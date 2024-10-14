/*--------------------------------------------------
        - ������ģ��(HC-SR04)�ײ��������� -
��Ƭ���ͺţ�STM32F103C8T6
---------------------------------------------------*/
#include "ultrasonic.h"
#include "delay.h"
uint8_t msHcCount = 0; //ms����


/**
  * @brief  ������ģ�����ų�ʼ��
  * @param  ��
  * @retval ��
  */
void ultrasonic_gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HCSR04_GPIO_CLK,ENABLE);
	/*------------������ģ�鴥������-------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Pin = HCSR04_GPIO_TRIG;  //TRIG����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HCSR04_GPIO_PORT,&GPIO_InitStructure);
	GPIO_ResetBits(HCSR04_GPIO_PORT,HCSR04_GPIO_TRIG); //Ĭ�ϳ�ʼΪ�͵�ƽ
	/*------------������ģ���������-------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //��������
	GPIO_InitStructure.GPIO_Pin = HCSR04_GPIO_ECHO; //ECHO����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HCSR04_GPIO_PORT,&GPIO_InitStructure); 
	GPIO_ResetBits(HCSR04_GPIO_PORT,HCSR04_GPIO_ECHO); //Ĭ�ϳ�ʼΪ�͵�ƽ
}


/**
  * @brief  ��������ʱ����ʼ�����ڼ������ŷ����źŸߵ�ƽʱ��
  * @param  uint16_t psc����ʱ��Ԥ��Ƶֵ uint16_t arr���Զ���װֵ
  * @retval ��
  */
void ultrasonic_TIMx_Init(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	/*------------��ʱ���ж����ȼ�����-------------*/
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = NVIC_Channel;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	/*-----------------��ʱ������-------------------*/
	HCSR04_TIMAPB_CLK(HCSR04_TIM_CLK,ENABLE);//��ʱ��ʱ�ӳ�ʼ��
	TIM_DeInit(TIMx); //��λ��ʱ��
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1; 
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure); 
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);   //��ʱ���ж�����
	TIM_ClearITPendingBit(TIMx,TIM_IT_Update); //�����ʱ���жϱ�־λ
	TIM_Cmd(TIMx,DISABLE); //��ʼ��ʱ������ʱ��
}


/**
  * @brief  ��������ʼ��
  * @param  ��
  * @retval ��
  */
void ultrasonic_Init(void)
{
	ultrasonic_gpio_Init();
	ultrasonic_TIMx_Init(72,1000);
}


/**
  * @brief  �򿪶�ʱ��������������ʱ����ʱ
  * @param  ��
  * @retval ��
  */
void OpenTimer(void)
{
	TIM_Cmd(TIMx,ENABLE); 
	msHcCount = 0;
	TIM_SetCounter(TIMx,0);
}


/**
  * @brief  �رն�ʱ��������ֹͣ��ʱ����ʱ
  * @param  ��
  * @retval ��
  */
void CloseTimer(void)
{
	TIM_Cmd(TIMx,DISABLE);
}


/**
  * @brief  ��ȡECHO���ŷ��ظߵ�ƽʱ��
  * @param  ��
  * @retval time���ߵ�ƽʱ��(us)
  */
uint32_t get_echoTime(void)
{
	uint32_t time = 0;
	time = msHcCount*1000; //��ʱ��1ms�ж�msHcCount��1��ת��us
	time = time + TIM_GetCounter(TIMx); //�����жϺ�ʱ��ʣ�����ֵ(us)
	TIM_SetCounter(TIMx,0);//������ֵ����
	DelayMs(10); //�˶���ʱ������
	//u1_printf("%d\r\n",time);
	return time;
}

/**
  * @brief  ��ȡ5��������ݵ�ƽ��ֵ�������
  * @param  ��
  * @retval ƽ������ֵ
  */
float get_AverageDistance(void)
{
	float length = 0;
	float length_sum = 0;
	for(int i=0;i<5;i++)
  {
		GPIO_SetBits(HCSR04_GPIO_PORT,HCSR04_GPIO_TRIG);
		DelayUs(100);
		GPIO_ResetBits(HCSR04_GPIO_PORT,HCSR04_GPIO_TRIG);
		while(GPIO_ReadInputDataBit(HCSR04_GPIO_PORT,HCSR04_GPIO_ECHO) == 0);
		OpenTimer();
		while(GPIO_ReadInputDataBit(HCSR04_GPIO_PORT,HCSR04_GPIO_ECHO) == 1);
		CloseTimer();
		length = get_echoTime();
		length_sum = length_sum + length/58;
	}
	return length_sum/5.0;
}

/**
  * @brief  ��ʱ���жϷ�����
  * @param  ��
  * @retval ��
  */
void TIMx_IRQHandler(void)
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update)==SET)
	{
		msHcCount++;
		TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
	}
}

