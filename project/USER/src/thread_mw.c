#include "main.h"

#define VERSION_V 1
#define VERSION_D 0

u8 printf_buf[]={
	0,		//ϵͳʱ��
	0,		//ϵͳ�汾
};

/*
��	�̣�
��	�ã�
Ƶ	�ʣ�1000Hz
��	�ڣ�
��	�ߣ�meetwit
*/
void loop(void* parameter){

}

/*
��	�̣�
��	�ã�
Ƶ	�ʣ�1000Hz
��	�ڣ�
��	�ߣ�meetwit
*/
void refresh_state(void* parameter){
	
}


/*
��	�̣�
��	�ã�
Ƶ	�ʣ�1000Hz
��	�ڣ�
��	�ߣ�meetwit
*/

void rc522(void* parameter){
	
}

/*
��	�̣�
��	�ã�
Ƶ	�ʣ�1000Hz
��	�ڣ�
��	�ߣ�meetwit
*/

void sensor(void* parameter){
	
}

/*
��	�̣�
��	�ã�
Ƶ	�ʣ�1000Hz
��	�ڣ�
��	�ߣ�meetwit
*/

void ds1302(void* parameter){
	
}

/*
��	�̣�
��	�ã�
Ƶ	�ʣ�1000Hz
��	�ڣ�
��	�ߣ�meetwit
*/

void save_t(void* parameter){

}

/*
�����ʱ��
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
��	�̣�
��	�ã�ͨ������1��ӡϵͳʱ��
Ƶ	�ʣ�1Hz
��	�ڣ�2019��11��22��
��	�ߣ�meetwit
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
