#ifndef __USART2_H
#define __USART2_H	 

#include "stm32f10x.h"

#define Rx_Max2 64
extern u8 Rx_Buf2[Rx_Max2];
extern u16 Rx_End2,Rx_Len2,Rx_Tm2;

void usart2_init(u32 baud);	//≥ı ºªØ		
void PcTx_Byte2(u8 Tx_data);
void PcTx_String2(u8 *str);
void Task_Pc2(void);


#endif

















