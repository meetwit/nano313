#include "thread_mw.h"

#define VERSION_V 1
#define VERSION_D 0

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/
void loop(void* parameter){
	
	while(1){
		
		rt_thread_delay(1);
		rt_timer_check();
	}
}

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/
void refresh_state(void* parameter){
	
	while(1){
		
		rt_thread_delay(1);
		rt_timer_check();
	}
}

/*
软件定时器
*/
void timer1_f(void* parameter){
	rt_timer_stop(timer1);
//	if(Rx_Tm1)			Rx_Tm1--;	
//	if(Rx_Tm1==1)		Task_Pc1();
	
	rt_timer_start(timer1);
}

/*
线	程：
作	用：通过串口1打印系统时间
频	率：1Hz
日	期：2019年11月22日
作	者：meetwit
*/
void time_thread(void* parameter){
	rt_tick_t tick_temp;
  rt_uint8_t h=0,m=0,s=0;

	while(1){
		tick_temp = rt_tick_get();
		s=tick_temp/RT_TICK_PER_SECOND%60;
		m=tick_temp/RT_TICK_PER_SECOND/60%60;
		h=tick_temp/RT_TICK_PER_SECOND/60/60%24;
		rt_kprintf("\r\nThe system runtime is %d:%d:%d.%d\r\n",h,m,s,(int)(tick_temp%RT_TICK_PER_SECOND));
		rt_kprintf("q%dqw%dw SOFT VERSION = V%d.%d\r\n",VERSION_V,VERSION_D,VERSION_V,VERSION_D);
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
	
}

