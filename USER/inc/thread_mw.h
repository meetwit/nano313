#ifndef __THREAD_MW_H
#define __THREAD_MW_H
#include <rtthread.h>
#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"

extern rt_timer_t timer1;

void loop(void* parameter);
void refresh_state(void* parameter);
void timer1_f(void* parameter);
void time_thread(void* parameter);

#endif
