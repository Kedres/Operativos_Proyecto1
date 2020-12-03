/*
 * mypthreads.h
 * 
 * Copyright 2020 Carlos <carlosm03@carlos-VB>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef MYPTHREADS_H
#define	MYPTHREADS_H

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "DataStructures.h"


//FUNCIONES DE THREADS

void my_thread_init(long pTimeInterval);
int my_thread_create(thread *pThread, void *(*pStartRoutine)(void *), void *pArgument, int pLimitTime, char *pSchedulerType);
thread my_thread_self(void);
void my_thread_yield(void);
void my_thread_exit(void *pReturnValue);
void my_thread_chsched(HN pThread);
void my_thread_sleep(long pSleepTime);
int my_thread_cancel(thread pThread);
int my_thread_join(thread pThread, void **pStatus);
int my_thread_detach(thread pThread);
long my_threadGetTimeExecution(thread pThread);

//FUNCIONES DE MUTEX

int my_mutex_init(HiloMutex *pMutex);
int my_mutex_lock(HiloMutex *pMutex);
int my_mutex_unlock(HiloMutex *pMutex);


#endif
