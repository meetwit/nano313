#include "main.h"

char Rx_Buf1[Rx_Max1];
u16 Rx_End1,Rx_Len1,Rx_Tm1;

///************************************************************
//函数名：void usart_init(u32 baud);
//形  参：u32 baud:波特率
//返回值：无
//功  能：串口1初始化
//作  者：meetwit
//时  间：2017年11月13日15:15:43
//*************************************************************/
void usart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

u16 usart_auto_init(void* parameter){
	usart1_init(921600);
	return 0;
}
INIT_BOARD_EXPORT(usart_auto_init);


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
							USART_SendData(USART1, a);//向串口1发送数据
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
        }
							USART_SendData(USART1, *(str + i));//向串口1发送数据
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
    }
}


char rt_hw_console_getchar(void)
{
	static u8 len,index;
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
