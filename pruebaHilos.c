/*
 * pruebaHilos.c
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


#include <stdio.h>
#include "mypthreads.h"

static void *imprimir(void * numero1);
static void *imprimir2(void * numero1);


int main(int argc, char **argv)
{
	
	thread *threadsArray;
	HiloMutex generalMutex;
	HiloMutex useSocket;
	
	char* s = "Sort";
	
	threadsArray = (thread*)malloc(sizeof(thread) * 2);
	
	my_thread_init(10);
	my_mutex_init(&generalMutex);
	my_mutex_init(&useSocket);
	
	my_thread_create(&threadsArray[0],imprimir,(void*)10,(int)150,s);
	//my_thread_yield();
	//my_thread_exit(0);
	
	//my_thread_create(&threadsArray[1],imprimir2,(void*)10,(int)150,s);
	
	my_thread_join(threadsArray[0],NULL);
	//my_thread_join(threadsArray[1],NULL);
	
	printf("\nhola mundo2\n");
	
	
	return 0;
}

static void *imprimir(void * numero1){
	
	char numero[10];
	scanf("%s",numero);
	printf("\ntecla : %s\n",numero);
	
	printf("\nhola mundo\n");
	//my_thread_yield();
	//my_thread_exit(0);
	
	//printf("\nhola mundo\n");
	return 0;
	
}

static void *imprimir2(void * numero1){
	/*
	char numero[10];
	scanf("%s",numero);
	printf("\ntecla : %s\n",numero);
	*/
	printf("\nhola 2\n");
	return 0;
	
}
