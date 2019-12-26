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
	u8 login;			//		 = 0 û�е�¼
								//		 = 1 ��½�ɹ�
								//		 = 2 ��½ʧ��
	u8 mode_JG;		//mode = 0 û�п�ʼ
								//		 = 1 ��ʼ
								//		 = 2 ��ʼ�����״̬
								//		 = 3 ��ͣ
								//		 = 4 ��ͣ��ȴ�״̬
								//		 = 5 �������
								//		 = 6 ��ʱ�˳�
	u8 mode_RGB;	//mode = 0 û�п�ʼ
								//		 = 1 ��ʼ
								//		 = 2 ��ʼ�����״̬
								//		 = 3 ��ͣ
								//		 = 4 ��ͣ��ȴ�״̬
								//		 = 5 �������
								//		 = 6 ��ʱ�˳�
	u16 mode_time;				//ģʽʣ��ʱ�䣬��
	u16 mode_timeout;			//ģʽ��ʱʱ�䣬��
	rt_tick_t start_tick;	//��¼ģʽ��ʼ��ʱ��
	rt_tick_t stop_tick;	//��¼ģʽ��ͣ��ʱ��
	u16 mode_time2;				//ģʽʣ��ʱ�䣬��
	u16 mode_timeout2;			//ģʽ��ʱʱ�䣬��
	rt_tick_t start_tick2;	//��¼ģʽ��ʼ��ʱ��
	rt_tick_t stop_tick2;	//��¼ģʽ��ͣ��ʱ��
	u8 sensor_state;			//0δ֪ 1���� 2���� 
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

















