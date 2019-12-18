#include "usart1.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>

char Rx_Buf1[Rx_Max1];
u16 Rx_End1,Rx_Len1,Rx_Tm1;

/************************************************************
函数名：void usart_init(u32 baud);
形  参：u32 baud:波特率
返回值：无
功  能：串口1初始化
作  者：meetwit
时  间：2017年11月13日15:15:43
*************************************************************/
void usart1_init(u32 baud){
	/*
	1.开GPIO时钟
	2.初始化PA9(发送	复用推挽)  PA10(接收	浮空输入)
	3.开串口1时钟
	4.字长：1个起始位，8个数据位，1个停止位
	5.发送使能
	6.波特率
	7.串口使能
	*/
	float div,div_frr;
	u16 div_man;
	
	RCC ->APB2ENR |= (0x1<<2);//GPIOA使能
	GPIOA ->CRH &=~(0xF<<4);
	GPIOA ->CRH |=(0xB<<4);//复用推挽
	
	GPIOA ->CRH &=~(0Xf<<8);
	GPIOA ->CRH |=(0X8<<8);//浮空输入
	
	RCC ->APB2ENR |=(0x1<<14);//串口1时钟使能
	USART1->CR1 &=~(0x1<<12);//一个起始位，8个数据位
	USART1->CR2 &=~(0X3<<12);//一个停止位
	USART1->CR1 |=(0x1<<3);//发送使能
	
	USART1->CR1|=(0x1<<2);    //接收使能
	USART1->CR1|=(1<<5);    //当USART_SR中的ORE或者RXNE为’1’时，产生USART中断
	
	div=(float) 72000000/(baud * 16);
	div_man =div;
	div_frr=div-div_man;
	div_man <<= 4;
	
	USART1->BRR = div_man + (div_frr*16);
	
	/*设置优先级*/
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,2,1));
	/*使能中断*/
	NVIC_EnableIRQ(USART1_IRQn);//NVIC_EnableIRQ是用来使能外部中断的
	
	USART1->CR1 |=(0x1<<13);//串口使能	
}

int init_u(void){
	usart1_init(921600);
	return 0;
}
INIT_BOARD_EXPORT(init_u);


/************************************************************
函数名：PcTx_Byte(u8 Tx_data)
形  参：u8 Tx_data
返回值：无
功  能：串口1发送一个字节
作  者：meetwit
时  间：2017年11月13日15:48:50
*************************************************************/
void PcTx_Byte1(u8 Tx_data){
	while(!(USART1->SR &(0x1<<7)));
	USART1 ->DR = Tx_data;
	
}

/************************************************************
函数名：PcTx_String(u8 *str)
形  参：u8 *str 字符串地址
返回值：无
功  能：串口1发送字符串
作  者：meetwit
时  间：2017年11月13日15:48:50
*************************************************************/
void PcTx_String1(u8 *str){
		while(*str !='\0'){
			PcTx_Byte1(*(str++));
		}
}

/************************************************************
函数名：USART1_IRQHandler()
形  参：无
返回值：无
功  能：串口2中断
作  者：meetwit
时  间：2017年11月19日11:27:43
*************************************************************/
	void USART1_IRQHandler(void)
{
	rt_interrupt_enter();
	u8 tempdata1;
	if(USART1->SR&(0x1<<5))	//接收到数据
	{ 
		tempdata1=USART1->DR;
		if((Rx_End1==0)&&(Rx_Len1<Rx_Max1)){
				Rx_Buf1[Rx_Len1++]=tempdata1;
				Rx_Tm1=5;//每次重新赋值5
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


