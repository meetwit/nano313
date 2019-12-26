#ifndef __USART1_H
#define __USART1_H	 

#include "stm32f10x.h"

#define Rx_Max1 64

extern char Rx_Buf1[Rx_Max1];
extern u16 Rx_End1,Rx_Len1,Rx_Tm1;

void usart1_init(u32 baud);	//≥ı ºªØ		
void PcTx_Byte1(u8 Tx_data);
void PcTx_String1(u8 *str);


#endif

















