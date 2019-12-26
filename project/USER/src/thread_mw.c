#include "main.h"

#define VERSION_V 1
#define VERSION_D 0

u8 printf_buf[]={
	0,		//系统时间
	0,		//系统版本
};

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/
void loop(void* parameter){

}

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/
void refresh_state(void* parameter){
	
}


/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/

void rc522(void* parameter){
	
}

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/

void sensor(void* parameter){
	
}

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/

void ds1302(void* parameter){
	
}

/*
线	程：
作	用：
频	率：1000Hz
日	期：
作	者：meetwit
*/

void save_t(void* parameter){

}

/*
软件定时器
*/
void timer1_f(void* parameter){
	rt_timer_stop(timer1);
	
	if(Rx_Tm2)			Rx_Tm2--;	
	if(Rx_Tm2==1)		Task_Pc2();
	
	if(Rx_Tm3)			Rx_Tm3--;	
	if(Rx_Tm3==1)		Task_Pc3();
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
		if(printf_buf[0]) rt_kprintf("\r\nThe system runtime is %d:%d:%d.%d\r\n",h,m,s,(int)(tick_temp%RT_TICK_PER_SECOND));
		if(printf_buf[1]) rt_kprintf("q%dqw%dw SOFT VERSION = V%d.%d\r\n",VERSION_V,VERSION_D,VERSION_V,VERSION_D);
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
	
}

static void sys_printf(int argc, char**argv)
{
    if (argc < 2)
    {
        rt_kprintf("Please input'sys_printf <open|close>'\n");
        return;
    }

    if (!rt_strcmp(argv[1], "open"))
    {
//        rt_kprintf("AT server!\n");
					printf_buf[0] = 1;
    }
    else if (!rt_strcmp(argv[1], "close"))
    {
//        rt_kprintf("AT client!\n");
					printf_buf[0] = 0;
    }
    else
    {
        rt_kprintf("Please input'atcmd <server|client>'\n");
    }
}

MSH_CMD_EXPORT(sys_printf, atcmd sample: sys_printf <open|close>);
