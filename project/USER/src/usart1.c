#include "main.h"

char Rx_Buf1[Rx_Max1];
u16 Rx_End1,Rx_Len1,Rx_Tm1;

///************************************************************
//��������void usart_init(u32 baud);
//��  �Σ�u32 baud:������
//����ֵ����
//��  �ܣ�����1��ʼ��
//��  �ߣ�meetwit
//ʱ  �䣺2017��11��13��15:15:43
//*************************************************************/
void usart1_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

u16 usart_auto_init(void* parameter){
	usart1_init(921600);
	return 0;
}
INIT_BOARD_EXPORT(usart_auto_init);


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
							USART_SendData(USART1, a);//�򴮿�1��������
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
        }
							USART_SendData(USART1, *(str + i));//�򴮿�1��������
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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
