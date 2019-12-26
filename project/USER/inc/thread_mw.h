#ifndef __THREAD_MW_H
#define __THREAD_MW_H
#include <rtthread.h>
#include "stdio.h"
#include "stm32f10x.h"
#include "string.h"

void loop(void* parameter);
void refresh_state(void* parameter);
void ds1302(void* parameter);
void rc522(void* parameter);
void sensor(void* parameter);
void save_t(void* parameter);
void timer1_f(void* parameter);
void time_thread(void* parameter);

#endif
