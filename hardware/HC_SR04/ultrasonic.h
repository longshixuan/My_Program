#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "sys.h"

//超声波模块引脚定义
#define HCSR04_GPIO_CLK   RCC_APB2Periph_GPIOA
#define HCSR04_GPIO_PORT  GPIOA
#define HCSR04_GPIO_TRIG  GPIO_Pin_6
#define HCSR04_GPIO_ECHO  GPIO_Pin_7
//超声波模块定时器相关定义
#define HCSR04_TIMAPB_CLK RCC_APB1PeriphClockCmd
#define HCSR04_TIM_CLK    RCC_APB1Periph_TIM4
#define TIMx              TIM4
//定时器中断相关配置
#define NVIC_Channel      TIM4_IRQn
#define TIMx_IRQHandler   TIM4_IRQHandler

void ultrasonic_Init(void);
float get_AverageDistance(void);

#endif



