
#include "usart.h"

extern unsigned char buffer[100];
u8 rxd_buf[6];
u8 rxd_flag=0;		//������ɱ�־λ
u8 rxd_index=0;		//�����ֽ�����
extern u8 temp_th;
extern u8 open_time,stop_time;
/*******************************************************************************
** ��������: fputc
** ��������:
** ����˵��: ch: [����/��]
**			 f: [����/��]
** ����˵��: None
** ������Ա: temp_duan
** ��������: 2019-07-09
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/
int fputc(int ch, FILE *f)
{
    /* �ȴ�������� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    /* ����һ���ֽ����ݵ�USART1 */
    USART_SendData(USART1, (uint8_t) ch);
    return (ch);
}


/*******************************************************************************
** ��������: fgetc
** ��������:
** ����˵��: f: [����/��]
** ����˵��: None
** ������Ա: temp_duan
** ��������: 2019-07-09
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/
int fgetc(FILE *f)
{
    /* �ȴ�����1�������� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);

}

/*******************************************************************************
** ��������: usart_init
** ��������:
** ����˵��: val_1: [����/��]
** ����˵��: None
** ������Ա: temp_duan
** ��������: 2019-07-09
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/
void usart_init(u32 val_1)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = val_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    /* ʹ�ܴ���1�����ж� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);



    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
** ��������: USART1_IRQHandler
** ��������:
** ����˵��: None
** ����˵��: None
** ������Ա: temp_duan
** ��������: 2019-07-09
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/
void USART1_IRQHandler(void)
{

        u8 res;
				static u8 res_state=0;
				if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
				{
					res=USART_ReceiveData(USART1);
					switch(res_state)
					{
						case 0:
							switch(res)
							{
								case 'T':res_state=1;
									break;
								case 'P':res_state=2;
									break;
								case 'S':res_state=3;
									break;
								default:res_state=0;
								break;
									
							}							
							break;
						case 1:
							rxd_buf[rxd_index]=res;
							rxd_index++;
							res_state=4;
							break;
						case 2:
							rxd_buf[rxd_index]=res;
							rxd_index++;
							res_state=5;
							break;
						case 3:
							rxd_buf[rxd_index]=res;
							rxd_index++;
							res_state=6;
							break;
						case 4:
							if(res==0x0D)
							{
								temp_th=rxd_buf[0]-48;
								rxd_index=0;
								res_state=0;
							}
							else
							{
								rxd_buf[rxd_index]=res;
								rxd_index=0;
								temp_th=(rxd_buf[0]-48)*10+(rxd_buf[1]-48);
								res_state=0;
							}
							break;
							case 5:
							if(res==0x0D)
							{
								open_time=rxd_buf[0]-48;
								rxd_index=0;
								res_state=0;
							}
							else
							{
								rxd_buf[rxd_index]=res;
								rxd_index=0;
								open_time=(rxd_buf[0]-48)*10+(rxd_buf[1]-48);
								res_state=0;
							}
							break;
							case 6:
							if(res==0x0D)
							{
								stop_time=rxd_buf[0]-48;
								rxd_index=0;
								res_state=0;
							}
							else
							{
								rxd_buf[rxd_index]=res;
								rxd_index=0;
								stop_time=(rxd_buf[0]-48)*10+(rxd_buf[1]-48);
								res_state=0;
							}
							break;


					}
				}
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);          /* Clear the USART Receive interrupt */
    

}
