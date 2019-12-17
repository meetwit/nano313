#include "thread_mw.h"

u16 time_x=0,time_xx=0;		//¼ÇÂ¼hzÊý




void get_adc(void* parameter){
	while(1){
		time_x++;
		
		rt_thread_delay(10);
		rt_timer_check();
	}
	
}

void send_data(void* parameter){
	
	while(1){
		time_xx++;
		
		rt_thread_delay(10);
		rt_timer_check();
	}
	
}

void time_thread(void* parameter){
	rt_tick_t tick_temp;
  rt_uint8_t h=0,m=0,s=0;
	
	while(1){
		tick_temp = rt_tick_get();
		s=tick_temp/RT_TICK_PER_SECOND%60;
		m=tick_temp/RT_TICK_PER_SECOND/60%60;
		h=tick_temp/RT_TICK_PER_SECOND/60/60%24;
		rt_kprintf("\r\nThe system runtime is %d:%d:%d.%d\r\n",h,m,s,tick_temp%RT_TICK_PER_SECOND);
		rt_kprintf("send_hz=%d,meg_hz=%d\r\n",time_x,time_xx);
		time_x=0;
		time_xx=0;
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
	
}
