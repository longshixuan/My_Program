
#include "TIM2.h"
#define SYSTICKPERIOD 0.000001

#define SYSTICKFREQUENCY (1/SYSTICKPERIOD)

void TIM2_Init(void)

{

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;



/*AHB = 72MHz,RCC_CFGR��PPRE1 = 2������APB1 = 36MHz,TIM2CLK = APB1*2 = 72MHz */

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

/* Time base configuration */

TIM_TimeBaseStructure.TIM_Period = 999;

TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/SYSTICKFREQUENCY -1;

TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

TIM_ARRPreloadConfig(TIM2, ENABLE);

/* ���ø�������Դֻ�ڼ��������������ʱ�����ж� */

TIM_UpdateRequestConfig(TIM2,TIM_UpdateSource_Global);

TIM_ClearFlag(TIM2, TIM_FLAG_Update);

}



/**

* @brief ms��ʱ����,1msΪһ����λ

* @param

* @arg nTime: Delay_ms( 10 ) ��ʵ�ֵ���ʱΪ 10 * 1ms = 10ms

* @retval ��

*/

void Delay_ms(__IO uint32_t nTime)

{

/* �����������ʹ�ܵδ�ʱ�� */

TIM2->CNT = 0;

TIM_Cmd(TIM2, ENABLE);



for( ; nTime > 0 ; nTime--)

{

/* �ȴ�һ����ʱ��λ�Ľ��� */

while(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != SET);

TIM_ClearFlag(TIM2, TIM_FLAG_Update);

}



TIM_Cmd(TIM2, DISABLE);

}

