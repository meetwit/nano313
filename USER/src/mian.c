#include "thread_mw.h"

/*将系统任务优先级设置大于finsh控制台(默认时6)，否则出现线程无法获取CPU的情况*/

rt_thread_t tid1=RT_NULL;
rt_thread_t tid2=RT_NULL;
rt_thread_t tid3=RT_NULL;
rt_thread_t tid4=RT_NULL;
rt_thread_t tid5=RT_NULL;
rt_thread_t tid6=RT_NULL;
rt_thread_t tid_time=RT_NULL;

rt_timer_t timer1=RT_NULL;

int main(){
	rt_kprintf("\r\n\r\nmeetwit rtt systerm start init!\r\n\r\n");
	
	tid1=rt_thread_create("loop",loop,RT_NULL,128,6,100);
	rt_thread_startup(tid1);
	
	tid2=rt_thread_create("refresh_state",refresh_state,RT_NULL,512,4,100);
	rt_thread_startup(tid2);


	tid_time=rt_thread_create("tid_time",time_thread,RT_NULL,256,5,10);
	rt_thread_startup(tid_time);
	
	timer1 = rt_timer_create("t1",timer1_f,RT_NULL,5,RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	rt_timer_start(timer1);
	
	rt_kprintf("meetwit rtt systerm started!\r\n\r\n");
	
	return 0;
}







