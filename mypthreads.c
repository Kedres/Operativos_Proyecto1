/*
 * mypthreads.c
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


#include "Schedulers.h"
#include "DataStructures.h"
#include "mypthreads.h"


HNCola threadsQueue = NULL;
struct itimerval timeQuantum;
long timeInterval;
sigset_t sigProcMask;
HilosMuertosNodesCola deadThreadsQueue = NULL;
struct sigaction schedulerHandle;
ucontext_t exitContext;
time_t randomTimeSeed;



extern MutexCola mutexQueue;
extern int roundRobinControl;
extern int sorteoControl;

static void *wrapperFunction(void *(*pStartRoutine)(void *), void *pArgument);
static void clearBlockedThreads(HN pTargetThread);
static void setExitContext();
static void executeExitContext();
static void setSchedulerType(HN pThread, char *pSchedulerType);


//Threads functions
void my_thread_init(long pTimeInterval) 
{

	if (threadsQueue == NULL && deadThreadsQueue == NULL) 
	{
		sigemptyset(&sigProcMask);
		sigaddset(&sigProcMask, SIGPROF);
		deadThreadsQueue = createDeadTheadsNodesQueue();
		threadsQueue = createHNCola();
		if (deadThreadsQueue == NULL || threadsQueue == NULL) 
		{
			return;
		}
		else
		{
			srand((unsigned) time(&randomTimeSeed));
			timeInterval = pTimeInterval * 1000;
			threadsQueue->quantum = pTimeInterval;
			HN HNMain = createNewHN();
			getcontext(&(HNMain->hiloContext));
			setExitContext();
			HNMain->hiloContext.uc_link = &exitContext;
			//La linea de abajo indica que el thread principal es administrado por el scheduler RoundRobin.
			// TCBMain->roundRobin = 1;
			// roundRobinControl = 1;
			// Descomentar las lineas comentadas de abajo y comentar la linea de arriba si se quiere que el thread principal sea administrado por el scheduler Sort.
			HNMain->sorteo = 1;
			int nextTicket = searchEndTicket(threadsQueue);
			HNMain->initialTicket = nextTicket;
			HNMain->finalTicket = nextTicket;
			sorteoControl = 1;
			//
			threadsQueue->currentThread = HNMain;
			insertThread(threadsQueue, HNMain);
			memset(&schedulerHandle, 0, sizeof (schedulerHandle));
			schedulerHandle.sa_handler = &realTime;
			sigaction(SIGPROF, &schedulerHandle, NULL);
			//printf("\nMyThread: Biblioteca MyThread Inicializada...\n");
			timeQuantum.it_value.tv_sec = 0;
			timeQuantum.it_value.tv_usec = timeInterval;
			timeQuantum.it_interval.tv_sec = 0;
			timeQuantum.it_interval.tv_usec = timeInterval;
			setitimer(ITIMER_PROF, &timeQuantum, NULL);
		}
	}
}

int my_thread_create(thread *pThread, void *(*pStartRoutine)(void *), void *pArgument, int pLimitTime, char *pSchedulerType) 
{
	if (threadsQueue != NULL) 
	{
		sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
		HN newHN = createNewHN();
		getcontext(&(newHN->hiloContext));
		if (newHN == NULL) 
		{
			freeThread(newHN);
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			return -3;
		}
		else
		{
			if(pLimitTime > 0)
			{
				newHN->limitTime = pLimitTime;
			}
			newHN->hiloContext.uc_link = &exitContext;
			newHN->startQuantum = threadsQueue->quantums;
			setSchedulerType(newHN, pSchedulerType);
			makecontext(&(newHN->hiloContext), (void (*)()) wrapperFunction, 2, pStartRoutine, pArgument);
			//setcontext(&(newHN->hiloContext));
			*pThread = newHN->hiloID;
			printf("MyThread: Nuevo thread creado: %ld\n", newHN->hiloID);
			
			insertThread(threadsQueue, newHN);
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			return 0;
		}
	}
	else
	{
		return -2;
	}
}


int my_thread_join(thread pThread, void **pStatus) 
{
	
	//printf("\nestado del hilo: %d",pThread->HilosBlocked);
	
	//printf("\n hiloActual: %ld \n",pThread);
	
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	
	HN currentThread = threadsQueue->currentThread;
	HN targetThread = searchThread(pThread, threadsQueue);
	
	if (currentThread == targetThread || currentThread == NULL || (targetThread != NULL && targetThread->detach)) 
	{
		
		sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		return -1;
	}
	else
	{
		//printf("\naca2\n");
		
		if (targetThread == NULL || targetThread->HilosCompleted) 
		{
			
			
			HiloMuertoNode deadThreadNode = searchDeadThread(deadThreadsQueue, pThread);
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			if (deadThreadNode != NULL) 
			{
				if (pStatus != NULL) 
				{
					*pStatus = *(deadThreadNode->returnValue);
				}
				deleteDeadThread(deadThreadsQueue, pThread);
				return 0;
			} 
			else 
			{
				return -1;
			}
		}
		else
		{
			
			insertWaitingThread(targetThread, currentThread);
			
			int isBlocked = currentThread->HilosBlocked;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			//printf("\naca\n");
			while (isBlocked) 
			{
				
				isBlocked = currentThread->HilosBlocked;
			}
			
			sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
			HiloMuertoNode deadThreadNode = searchDeadThread(deadThreadsQueue, pThread);
			if(deadThreadNode != NULL)
			{
				
				if (pStatus != NULL) 
				{   
					*pStatus = *(deadThreadNode->returnValue);
				}
				if(((deadThreadNode->HilosEsperando) - 1) == 0)
				{   
					deleteDeadThread(deadThreadsQueue, pThread);
				}
				else
				{
					deadThreadNode->HilosEsperando--;
				}
				sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
				return 0;
			}
			else
			{
				//printf("MyThread: Un thread anterior a este ha realizado el join primero, intente realizando el join para ambos threads antes que el thread al cual desea hacer el join haya finalizado\n");
				sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
				return 0;
			}
		}
	}
}



thread my_thread_self(void) 
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	HN currentThread = threadsQueue->currentThread;
	if (currentThread == NULL) 
	{
		sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		return -4;
	}
	else
	{
		sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		return currentThread->hiloID;
	}
}

void my_thread_yield(void) 
{
	raise(SIGPROF);
}

int my_thread_cancel(thread pThread) 
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	HN currentThread = threadsQueue->currentThread;
	if ((currentThread != NULL) && (currentThread->hiloID != pThread)) 
	{
		HN targetThread = searchThread(pThread, threadsQueue);
		if (targetThread != NULL) 
		{
			clearBlockedThreads(targetThread);
			//printf("MyThread: Thread %ld cancelado\n", targetThread->threadID);
			targetThread->HilosCompleted = 1;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			return 0;
		}
		else
		{
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			return -1;
		}
	}
	else
	{
		sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		return -1;
	}
}



void my_thread_exit(void *pReturnValue) 
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	HN currentHN = threadsQueue->currentThread;
	HiloMuertoNode deadThreadNode = createNewDeadThreadNode();
	if (deadThreadNode != NULL && currentHN != NULL) 
	{
		*(deadThreadNode->returnValue) = pReturnValue;
		deadThreadNode->threadID = currentHN->hiloID;
		deadThreadNode->HilosEsperando = currentHN->HiloWaiting;
		insertDeadThred(deadThreadsQueue, deadThreadNode);
	}
	executeExitContext();
	sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
	raise(SIGPROF);
}

int my_thread_detach(thread pThread)
{
	HN targetThread = searchThread(pThread, threadsQueue);
	if(targetThread != NULL)
	{
		targetThread->detach = 1;
		return 0;
	}
	else
	{
		return -1;
	}
}

void my_thread_chsched(HN pThread)
{
	if(pThread->HilosCompleted == 0)
	{
		if(pThread->sorteo)
		{
			sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
			pThread->sorteo = 0;
			pThread->roundRobin = 1;
			restructureTickets(threadsQueue, pThread);
			pThread->initialTicket = 0;
			pThread->finalTicket = 0;
			if(threadsQueue->countRoundRobin == 0)
			{
				setNewHead(threadsQueue, pThread);
			}
			threadsQueue->countRoundRobin++;
			threadsQueue->countSorteo--;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		}
		else
		{
			sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
			pThread->roundRobin = 0;
			setSchedulerType(pThread, "Sort");
			moveHeadToNextRoundRobin(threadsQueue, pThread);
			threadsQueue->countRoundRobin--;
			threadsQueue->countSorteo++;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		}
	}
	else
	{
		//printf("MyThread: No puede cambiar de scheduler un thread que ya ha sido completado.\n");
	}
}



void my_thread_sleep(long pSleepTime)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    long actualQuantums = threadsQueue->quantums;
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    if(pSleepTime >= threadsQueue->quantum)
    {
        while(((threadsQueue->quantums - actualQuantums) * threadsQueue->quantum) <= pSleepTime);
    }
}

long my_threadGetTimeExecution(thread pThread)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    HN targetThread = searchThread(pThread, threadsQueue);
    long actualQuantums = threadsQueue->quantums;
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return (actualQuantums - targetThread->startQuantum) * threadsQueue->quantum;
}

static void setSchedulerType(HN pThread, char *pSchedulerType)
{
	if(!strcmp("Sort", pSchedulerType))
	{
		pThread->sorteo = 1;
        int nextTicket = searchEndTicket(threadsQueue);
        pThread->initialTicket = nextTicket;
        pThread->finalTicket = nextTicket;
	}
	else
	{
		pThread->roundRobin = 1;
	}
}

static void *wrapperFunction(void *(*pStartRoutine)(void *), void *pArgument) 
{
    void *returnValueFunction;
    HN currentHN = threadsQueue->currentThread;
    //printf("aca");
    returnValueFunction = (*pStartRoutine)(pArgument);
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(!currentHN->detach)
    {
    	HiloMuertoNode deadThreadNode = createNewDeadThreadNode();
	    if (deadThreadNode != NULL) 
	    {
	        *(deadThreadNode->returnValue) = returnValueFunction;
	        deadThreadNode->threadID = currentHN->hiloID;
	        deadThreadNode->HilosEsperando = currentHN->HiloWaiting;
	        insertDeadThred(deadThreadsQueue, deadThreadNode);
	    }
    }
    //
    //printDeadQueue(deadThreadsQueue);
    //
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return returnValueFunction;
}


static void clearBlockedThreads(HN pTargetThread) 
{
    HilosEspera blockedThread = pTargetThread->hilosEspera;
    while(blockedThread != NULL)
    {
        blockedThread->waitingThreadNode->HilosBlocked = 0;
        blockedThread = blockedThread->nextHiloEspera;
    }
}

static void executeExitContext()
{
    HN currentThread = threadsQueue->currentThread;
    clearBlockedThreads(currentThread);
    //printf("MyThread: Thread %ld completado\n", currentThread->threadID);
    currentThread->HilosCompleted = 1;
    raise(SIGPROF);
}

static void setExitContext()
{
    static int exitContextCreated;
    if(!exitContextCreated)
    {
        getcontext(&exitContext);
        exitContext.uc_link = 0;
        exitContext.uc_stack.ss_sp = malloc(STACKSIZE);
        exitContext.uc_stack.ss_size = STACKSIZE;
        exitContext.uc_stack.ss_flags= 0;
        makecontext(&exitContext, (void (*) (void))&executeExitContext, 0);
        exitContextCreated = 1;
    }
}

//Mutex Functions


int my_mutex_init(HiloMutex *pMutex)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(pMutex != NULL)
    {
        if(!isMutexInQueue(pMutex))
        {
            MutexCola newMutexQueue = createNewMutexQueue();
            newMutexQueue->mutex = createNewMutexNode();
            pMutex->lockNumber = newMutexQueue->mutex->lockNumber;
            insertMutexQueue(newMutexQueue);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return 0;
        }
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return -1;
}

int my_mutex_lock(HiloMutex *pMutex)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    MutexCola auxQueue = searchMutexQueue(pMutex);
    HN currentHN = threadsQueue->currentThread;
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    if(auxQueue == NULL)
    {        
        return -1;
    }
    else
    {
    	if(currentHN != NULL)
	    {
            while(auxQueue->threadNodeList);
            sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
            HilosEspera waitingHNNode = createWaitingThreadsList();
            waitingHNNode->waitingThreadNode = currentHN;
            waitingHNNode->nextHiloEspera = auxQueue->threadNodeList;
            auxQueue->threadNodeList = waitingHNNode;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return 0;
	    }
	    else
	    {
	    	sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
	    	return -1;
	    }
    }
}

int my_mutex_unlock(HiloMutex *pMutex)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    MutexCola auxQueue = searchMutexQueue(pMutex);
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    if(auxQueue == NULL)
    {   
        return -1;
    }
    else if(auxQueue->threadNodeList == NULL)
    {
        return -1;
    }
    else
    {
    	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
        HN currentHN = threadsQueue->currentThread;
        HilosEspera waitingHNNode = auxQueue->threadNodeList;
        if(waitingHNNode ->waitingThreadNode == currentHN)
        {
            auxQueue->threadNodeList = NULL;
            free(waitingHNNode );
        }
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
	    return 0;
    }
}
