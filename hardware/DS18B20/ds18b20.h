#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
#include "stdio.h"

/*��������֮���޸�����ط����ղ���PB0�ģ����ڸĳ�PA5�ģ�ֻ���޸�����ط��Ϳ�����*/
#define GPIO_CLOCK     		RCC_APB2Periph_GPIOB    //ʱ��
#define GPIO_WHAT       	GPIOB										//GPIO��
#define GPIO_PIN        	GPIO_Pin_6						//GPIO��
#define GPIO_PIN_DATA   	6							   				//���ǵڼ���
#define	DS18B20_DQ_OUT    PBout(6)                //���ݶ˿�	PA5 
#define	DS18B20_DQ_IN     PBin(6)                 //���ݶ˿�	PA5 
/*��������֮���޸�����ط����ղ���PB0�ģ����ڸĳ�PA5�ģ�ֻ���޸�����ط��Ϳ�����*/


#if (GPIO_PIN_DATA < 8 )
#define GPIO_DATA (0xFFFFFFFF & (~(0x0f << 4 * GPIO_PIN_DATA)))
#define DS18B20_IO_IN()  {GPIO_WHAT->CRL&=GPIO_DATA;GPIO_WHAT->CRL|=8<<4*GPIO_PIN_DATA;}
#define DS18B20_IO_OUT() {GPIO_WHAT->CRL&=GPIO_DATA;GPIO_WHAT->CRL|=3<<4*GPIO_PIN_DATA;}//X����4=20
#endif

#if (GPIO_PIN_DATA >= 8 )
#define GPIO_DATA (0xFFFFFFFF & (~(0x0f << 4 * (GPIO_PIN_DATA-8))))
#define DS18B20_IO_IN()  {GPIO_WHAT->CRH&=GPIO_DATA;GPIO_WHAT->CRH|=8<<4*(GPIO_PIN_DATA-8);}
#define DS18B20_IO_OUT() {GPIO_WHAT->CRH&=GPIO_DATA;GPIO_WHAT->CRH|=3<<4*(GPIO_PIN_DATA-8);}//X����4=20
#endif


   	
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#endif















