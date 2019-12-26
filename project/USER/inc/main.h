#ifndef __MAIN_MW_H
#define __MAIN_MW_H	 

#include "stdio.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"

#include <rtthread.h>
#include "thread_mw.h"

extern rt_timer_t timer1;

struct user{
	u8 login;			//		 = 0 没有登录
								//		 = 1 登陆成功
								//		 = 2 登陆失败
	u8 mode_JG;		//mode = 0 没有开始
								//		 = 1 开始
								//		 = 2 开始后进行状态
								//		 = 3 暂停
								//		 = 4 暂停后等待状态
								//		 = 5 正常完成
								//		 = 6 超时退出
	u8 mode_RGB;	//mode = 0 没有开始
								//		 = 1 开始
								//		 = 2 开始后进行状态
								//		 = 3 暂停
								//		 = 4 暂停后等待状态
								//		 = 5 正常完成
								//		 = 6 超时退出
	u16 mode_time;				//模式剩余时间，秒
	u16 mode_timeout;			//模式超时时间，秒
	rt_tick_t start_tick;	//记录模式开始的时间
	rt_tick_t stop_tick;	//记录模式暂停的时间
	u16 mode_time2;				//模式剩余时间，秒
	u16 mode_timeout2;			//模式超时时间，秒
	rt_tick_t start_tick2;	//记录模式开始的时间
	rt_tick_t stop_tick2;	//记录模式暂停的时间
	u8 sensor_state;			//0未知 1靠近 2分离 
	u16 DEF_T;			  
	u16 DEF_T_OUT;	
	u16	red_flag;
};	
extern struct user 		user1;

//extern u16 his_d[2000];
extern u16 his_s[20];
extern u16 his_z[20];
extern u16 his_base[20];

#endif

















