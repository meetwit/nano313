#include "usart1.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>

char Rx_Buf1[Rx_Max1];
u16 Rx_End1,Rx_Len1,Rx_Tm1;

/************************************************************
��������void usart_init(u32 baud);
��  �Σ�u32 baud:������
����ֵ����
��  �ܣ�����1��ʼ��
��  �ߣ�meetwit
ʱ  �䣺2017��11��13��15:15:43
*************************************************************/
void usart1_init(u32 baud){
	/*
	1.��GPIOʱ��
	2.��ʼ��PA9(����	��������)  PA10(����	��������)
	3.������1ʱ��
	4.�ֳ���1����ʼλ��8������λ��1��ֹͣλ
	5.����ʹ��
	6.������
	7.����ʹ��
	*/
	float div,div_frr;
	u16 div_man;
	
	RCC ->APB2ENR |= (0x1<<2);//GPIOAʹ��
	GPIOA ->CRH &=~(0xF<<4);
	GPIOA ->CRH |=(0xB<<4);//��������
	
	GPIOA ->CRH &=~(0Xf<<8);
	GPIOA ->CRH |=(0X8<<8);//��������
	
	RCC ->APB2ENR |=(0x1<<14);//����1ʱ��ʹ��
	USART1->CR1 &=~(0x1<<12);//һ����ʼλ��8������λ
	USART1->CR2 &=~(0X3<<12);//һ��ֹͣλ
	USART1->CR1 |=(0x1<<3);//����ʹ��
	
	USART1->CR1|=(0x1<<2);    //����ʹ��
	USART1->CR1|=(1<<5);    //��USART_SR�е�ORE����RXNEΪ��1��ʱ������USART�ж�
	
	div=(float) 72000000/(baud * 16);
	div_man =div;
	div_frr=div-div_man;
	div_man <<= 4;
	
	USART1->BRR = div_man + (div_frr*16);
	
	/*�������ȼ�*/
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,2,1));
	/*ʹ���ж�*/
	NVIC_EnableIRQ(USART1_IRQn);//NVIC_EnableIRQ������ʹ���ⲿ�жϵ�
	
	USART1->CR1 |=(0x1<<13);//����ʹ��	
}

int init_u(void){
	usart1_init(921600);
	return 0;
}
INIT_BOARD_EXPORT(init_u);


/************************************************************
��������PcTx_Byte(u8 Tx_data)
��  �Σ�u8 Tx_data
����ֵ����
��  �ܣ�����1����һ���ֽ�
��  �ߣ�meetwit
ʱ  �䣺2017��11��13��15:48:50
*************************************************************/
void PcTx_Byte1(u8 Tx_data){
	while(!(USART1->SR &(0x1<<7)));
	USART1 ->DR = Tx_data;
	
}

/************************************************************
��������PcTx_String(u8 *str)
��  �Σ�u8 *str �ַ�����ַ
����ֵ����
��  �ܣ�����1�����ַ���
��  �ߣ�meetwit
ʱ  �䣺2017��11��13��15:48:50
*************************************************************/
void PcTx_String1(u8 *str){
		while(*str !='\0'){
			PcTx_Byte1(*(str++));
		}
}

/************************************************************
��������USART1_IRQHandler()
��  �Σ���
����ֵ����
��  �ܣ�����2�ж�
��  �ߣ�meetwit
ʱ  �䣺2017��11��19��11:27:43
*************************************************************/
	void USART1_IRQHandler(void)
{
	rt_interrupt_enter();
	u8 tempdata1;
	if(USART1->SR&(0x1<<5))	//���յ�����
	{ 
		tempdata1=USART1->DR;
		if((Rx_End1==0)&&(Rx_Len1<Rx_Max1)){
				Rx_Buf1[Rx_Len1++]=tempdata1;
				Rx_Tm1=5;//ÿ�����¸�ֵ5
		}
	}		
	rt_interrupt_leave();
}


void rt_hw_console_output(const char *str)
{
	rt_size_t i = 0, size = 0;
    char a = '\r';
	size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
					PcTx_Byte1(a);
        }
				PcTx_Byte1(*(str + i));
    }

}

char rt_hw_console_getchar(void)
{
	static u8 len=0,index=0;
  int ch = -1;
	if(Rx_Tm1){
		Rx_Tm1--;
		rt_thread_delay(10);
	}	
	if(Rx_Tm1==1){
		len = Rx_Len1;
		index = 0;
	}
	if(Rx_Tm1==0&&len){
		len--;
		ch = Rx_Buf1[index];
		index++;
		
		if(len==0){
			Rx_End1=0;
			Rx_Len1=0;
			memset(Rx_Buf1,0,sizeof(Rx_Buf1));
		}
	}
  return ch;
}


