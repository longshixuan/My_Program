/*--------------------------------------------------
        - 超声波模块(HC-SR04)底层驱动代码 -
单片机型号：STM32F103C8T6
---------------------------------------------------*/
#include "ultrasonic.h"
#include "delay.h"
uint8_t msHcCount = 0; //ms计数


/**
  * @brief  超声波模块引脚初始化
  * @param  无
  * @retval 无
  */
void ultrasonic_gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HCSR04_GPIO_CLK,ENABLE);
	/*------------超声波模块触发引脚-------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Pin = HCSR04_GPIO_TRIG;  //TRIG引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HCSR04_GPIO_PORT,&GPIO_InitStructure);
	GPIO_ResetBits(HCSR04_GPIO_PORT,HCSR04_GPIO_TRIG); //默认初始为低电平
	/*------------超声波模块接收引脚-------------*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //下拉输入
	GPIO_InitStructure.GPIO_Pin = HCSR04_GPIO_ECHO; //ECHO引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HCSR04_GPIO_PORT,&GPIO_InitStructure); 
	GPIO_ResetBits(HCSR04_GPIO_PORT,HCSR04_GPIO_ECHO); //默认初始为低电平
}


/**
  * @brief  超声波定时器初始化用于计算引脚返回信号高电平时间
  * @param  uint16_t psc：定时器预分频值 uint16_t arr：自动重装值
  * @retval 无
  */
void ultrasonic_TIMx_Init(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	/*------------定时器中断优先级配置-------------*/
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = NVIC_Channel;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	/*-----------------定时器配置-------------------*/
	HCSR04_TIMAPB_CLK(HCSR04_TIM_CLK,ENABLE);//定时器时钟初始化
	TIM_DeInit(TIMx); //复位定时器
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1; 
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure); 
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);   //定时器中断配置
	TIM_ClearITPendingBit(TIMx,TIM_IT_Update); //清除定时器中断标志位
	TIM_Cmd(TIMx,DISABLE); //初始化时不开定时器
}


/**
  * @brief  超声波初始化
  * @param  无
  * @retval 无
  */
void ultrasonic_Init(void)
{
	ultrasonic_gpio_Init();
	ultrasonic_TIMx_Init(72,1000);
}


/**
  * @brief  打开定时器，用于启动定时器计时
  * @param  无
  * @retval 无
  */
void OpenTimer(void)
{
	TIM_Cmd(TIMx,ENABLE); 
	msHcCount = 0;
	TIM_SetCounter(TIMx,0);
}


/**
  * @brief  关闭定时器，用于停止定时器计时
  * @param  无
  * @retval 无
  */
void CloseTimer(void)
{
	TIM_Cmd(TIMx,DISABLE);
}


/**
  * @brief  获取ECHO引脚返回高电平时间
  * @param  无
  * @retval time：高电平时间(us)
  */
uint32_t get_echoTime(void)
{
	uint32_t time = 0;
	time = msHcCount*1000; //定时器1ms中断msHcCount加1，转成us
	time = time + TIM_GetCounter(TIMx); //加上中断后定时器剩余计数值(us)
	TIM_SetCounter(TIMx,0);//将计数值清零
	DelayMs(10); //此段延时不可少
	//u1_printf("%d\r\n",time);
	return time;
}

/**
  * @brief  获取5组距离数据的平均值减少误差
  * @param  无
  * @retval 平均距离值
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
  * @brief  定时器中断服务函数
  * @param  无
  * @retval 无
  */
void TIMx_IRQHandler(void)
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update)==SET)
	{
		msHcCount++;
		TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
	}
}

