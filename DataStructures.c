/*
 * DataStructures.c
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


#include "DataStructures.h"
//variables globales

//Hilos

static long actHiloID = 0;



//Mutex

MutexCola mutexCola = NULL;



//Funciones del sistema de hilos


static int getHiloID()
{
    
    
    return ++actHiloID;
}


HN getHeadHN(HNCola pQueue)
{
    if(pQueue == NULL)
    {
        return NULL;
    }
    else
    {
    	return pQueue->head;
    }
}


HN getNextHN(HNCola pQueue)
{
    if(pQueue != NULL && pQueue->head != NULL)
    {
        return pQueue->head->nextHilo;
    }
    else
    {
    	return NULL;
    }
}


HN createNewHN(){
	
	HN newHN = (HN)malloc(sizeof(struct HiloNodes));
	if(newHN == NULL){
		free(newHN);
		return NULL;
	}
	else{
		
		newHN->hiloContext.uc_stack.ss_sp = malloc(STACKSIZE);
		if(newHN->hiloContext.uc_stack.ss_sp == NULL){
			
			free(newHN);
			return NULL;
			
		}
		else{
			
			newHN->hiloContext.uc_stack.ss_size = STACKSIZE;
			newHN->hiloContext.uc_stack.ss_flags = 0;
			newHN->nextHilo = NULL;
			newHN->hiloContext.uc_link = 0;
			newHN->HilosCompleted = 0;
			newHN->hilosEspera = NULL;
			newHN->HilosBlocked = 0;
			newHN->HiloWaiting = 0;
			newHN->roundRobin = 0;
			newHN->sorteo = 0;
			newHN->initialTicket = 0;
			newHN->finalTicket = 0;
			newHN->warningLevel = 0;
			newHN->ultimateWarningLevel = 0;
			newHN->limitTime = 0;
			newHN->detach = 0;
			newHN->startQuantum = 0;
			newHN->hiloID = getHiloID();
			return newHN;
		}
	}
}


HN searchThread(thread pThreadID, HNCola pQueue){
	
	HN headThread = getHeadHN(pQueue);
	
	if(headThread == NULL){
		return NULL;
	}
	
	else if(headThread->hiloID == pThreadID){
		return headThread;
	}
	
	else{
		HN temporalThread = headThread->nextHilo;
		
		while(headThread != temporalThread){
			if(temporalThread->hiloID == pThreadID){
				return temporalThread;
			}
			
			else{
				temporalThread = temporalThread->nextHilo;
			}
		}
		return NULL;
	}
}


HN searchThreadTicket(int pTicket, HNCola pQueue){
	
	HN headThread = getHeadHN(pQueue);
	
	if(headThread == NULL){
		return NULL;
	}
	else if(headThread->initialTicket <= pTicket && headThread->finalTicket >= pTicket){
		return headThread;
	}
	else{
		HN temporalThread = headThread->nextHilo;
		while(headThread != temporalThread){
			if(temporalThread->initialTicket <= pTicket && temporalThread->finalTicket >= pTicket){
				return temporalThread;
			}
			else{
				temporalThread = temporalThread->nextHilo;
			}
		}
		return NULL;  
	}
}


HNCola createHNCola(){
	HNCola newCola = (HNCola)malloc(sizeof(struct HNColas));
	if(newCola == NULL){
		return NULL;
	}
	else{
		newCola->quantum = 0;
		newCola->quantums = 0;
		newCola->count = 0;
		newCola->countRoundRobin = 0;
		newCola->countSorteo = 0;
		newCola->head = NULL;
		newCola->headParent = NULL;
		newCola->currentThread = NULL;
		newCola->currentThreadCopy = NULL;
		return newCola;
	}
}


HilosEspera createWaitingThreadsList()
{
	HilosEspera newList = (HilosEspera)malloc(sizeof(struct ListaHilosEspera));
	if(newList != NULL)
	{
		newList->waitingThreadNode = NULL;
		newList->nextHiloEspera = NULL;
	}
	return newList;
}

HiloMuertoNode searchDeadThread(HilosMuertosNodesCola pQueue, thread pThreadID)
{
	if(pQueue != NULL)
	{
		HiloMuertoNode deadThread = pQueue->head;
		while((deadThread != NULL) && (deadThread->threadID != pThreadID))
		{
			deadThread = deadThread->nextHiloMuertoNode;
		}
		return deadThread;
	}
	else
	{
		return NULL;
	}
}


void deleteDeadThread(HilosMuertosNodesCola pQueue, thread pThreadID)
{
	HiloMuertoNode  deadThread = pQueue->head;
	HiloMuertoNode  previousDeadTread = NULL;
	while((deadThread != NULL) && (deadThread->threadID != pThreadID))
	{
		previousDeadTread = deadThread;
		deadThread = deadThread->nextHiloMuertoNode;
	}
	if(deadThread != NULL)
	{
		if(previousDeadTread == NULL)
		{
			pQueue->head = deadThread->nextHiloMuertoNode;
		}
		else
		{
			previousDeadTread->nextHiloMuertoNode = deadThread->nextHiloMuertoNode;
		}
		free(deadThread);
	}
}


HiloMuertoNode createNewDeadThreadNode()
{
	HiloMuertoNode newDeadThread = (HiloMuertoNode)malloc(sizeof(struct HilosMuertosNodes));
	if(newDeadThread == NULL)
	{
		return NULL;
	}
	else
	{
		newDeadThread->returnValue = (void**)malloc(sizeof(void*));
		if(newDeadThread->returnValue == NULL)
		{
			free(newDeadThread);
			return NULL;
		}
		else
		{
			newDeadThread->threadID = -4;
			*(newDeadThread->returnValue) = NULL;
			newDeadThread->HilosEsperando = 0;
			newDeadThread->nextHiloMuertoNode = NULL;
			return newDeadThread;
		}
	}
}

HilosMuertosNodesCola createDeadTheadsNodesQueue()
{
	HilosMuertosNodesCola newQueue = (HilosMuertosNodesCola)malloc(sizeof(struct HiloMuertoNodeCola));
	if(newQueue == NULL)
	{
		return NULL;
	}
	newQueue->count = 0;
	newQueue->head = NULL;
	return newQueue;
}

int moveForward(HNCola pQueue)
{
	int result = -1;
	if(pQueue != NULL)
	{
		HN headThread = pQueue->head;
		if(headThread != NULL)
		{
			pQueue->head = headThread->nextHilo;
			pQueue->headParent = headThread;
			result = 0;
		}
	}
	return result;
}

int getHNCount(HNCola pQueue)
{
	if(pQueue == NULL)
	{
		return 0;
	}
	else
	{
		return pQueue->count;
	}
}




int getHNRoundRobinCount(HNCola pQueue)
{
	if(pQueue == NULL)
	{
		return 0;
	}
	else
	{
		return pQueue->countRoundRobin;
	}
}

int getHNSortCount(HNCola pQueue)
{
	if(pQueue == NULL)
	{
		return 0;
	}
	else
	{
		return pQueue->countSorteo;
	}
}

void freeThread(HN pThread)
{
	if(pThread != NULL)
	{
		HilosEspera waitingThreadNode = pThread->hilosEspera;
		free(pThread->hiloContext.uc_stack.ss_sp);
		while(waitingThreadNode != NULL)
		{
			HilosEspera nextWaitingThreadNode = waitingThreadNode->nextHiloEspera;
			free(waitingThreadNode);
			waitingThreadNode = nextWaitingThreadNode;
		}
		free(pThread);
	}
}

int deleteHeadThread(HNCola pQueue)
{
	int result = 0;
	HN previousThread, headThread;
	if(pQueue == NULL)
	{
		result = -1;
	}
	else
	{
		headThread = pQueue->head;
		previousThread = pQueue->headParent;
		if(headThread != NULL)
		{
			if(pQueue->count == 1)
			{
				pQueue->head = NULL;
				pQueue->headParent = NULL;
			}
			else
			{
				pQueue->head = headThread->nextHilo;
				previousThread->nextHilo = pQueue->head;
			}
			headThread->roundRobin == 1 ? pQueue->countRoundRobin-- : pQueue->countSorteo--;
			freeThread(headThread);
			pQueue->count--;
		}
		else
		{
			result = -1;
		}
	}
	return result;
}


int deleteSortThread(thread pThreadID, HNCola pQueue)
{
	int result = 0;
	HN headThread;
	if(pQueue == NULL)
	{
		result = -1;
	}
	else
	{
		headThread = pQueue->head;
		if(headThread != NULL)
		{
			if(pQueue->count == 1)
			{
				pQueue->head = NULL;
				pQueue->headParent = NULL;
			}
			else if(headThread->hiloID == pThreadID)
			{
				pQueue->headParent->nextHilo= headThread->nextHilo;
				pQueue->head = pQueue->headParent->nextHilo;
				if((pQueue->countSorteo - 1) != 0)
				{
					restructureTickets(pQueue, headThread);
				}
				free(headThread);
			}
			else
			{
				while(headThread->nextHilo->hiloID != pThreadID)
				{
					headThread = headThread->nextHilo;
				}
				HN auxThred = headThread->nextHilo;
				headThread->nextHilo = auxThred->nextHilo;
				if(headThread->nextHilo== pQueue->head)
				{
					pQueue->headParent = headThread;
				}
				if((pQueue->countSorteo - 1) != 0)
				{
					restructureTickets(pQueue, auxThred);
				}
				free(auxThred);
			}
			pQueue->count--;
			pQueue->countSorteo--;
		}
		else
		{
			result = -1;
		}
	}
	return result;
}



int insertThread(HNCola pQueue, HN pThread)
{
	int result = 0;
	if(pQueue == NULL || pThread == NULL)
	{
		result = -1;
	}
	else
	{
		if(pQueue->head == NULL)
		{
			
			printf("\n id a meter :%ld \n",pThread->hiloID);
			pThread->nextHilo = pThread;
			pQueue->headParent = pThread;
			pQueue->head = pThread;
		}
		else if(pQueue->countRoundRobin == 0)
		{
			pThread->nextHilo= pQueue->head;
			pQueue->headParent->nextHilo = pThread;
			pQueue->head = pThread;
		}
		else
		{
			pThread->nextHilo = pQueue->head;
			pQueue->headParent->nextHilo= pThread;
			pQueue->headParent = pThread;
		}
		pQueue->count++;
		pThread->roundRobin == 1 ? pQueue->countRoundRobin++ : pQueue->countSorteo++;
	}
	return result;
}

int insertWaitingThread(HN pTargetThread, HN pThreadAdded)
{
	HilosEspera newWaitingThread = createWaitingThreadsList();
	if(newWaitingThread != NULL)
	{
		pTargetThread->HiloWaiting++;
		newWaitingThread->waitingThreadNode = pThreadAdded;
		newWaitingThread->nextHiloEspera = pTargetThread->hilosEspera;
		pTargetThread->hilosEspera = newWaitingThread;
		pThreadAdded->HilosBlocked = 1;
		return 0;
	}
	else
	{
		return -1;
	}
}


int insertDeadThred(HilosMuertosNodesCola pQueue, HiloMuertoNode pThread)
{
	if(pQueue != NULL && pThread != NULL)
	{
		pThread->nextHiloMuertoNode = pQueue->head;
		pQueue->head = pThread;
		return 0;
	}
	else
	{
		return -1;
	}
}

int searchEndTicket(HNCola pQueue)
{
	HN headThread = getHeadHN(pQueue);
	if(headThread == NULL)
	{
		return 1;
	}
	else
	{
		int nextTicket = 0;
		HN nextThread = headThread->nextHilo;
		if(headThread->sorteo && headThread->finalTicket > nextTicket)
		{
			nextTicket = headThread->finalTicket;
		}
		while(nextThread != headThread)
		{
			if(nextThread->sorteo)
			{
				if(nextThread->finalTicket > nextTicket)
				{
					nextTicket = nextThread->finalTicket;
				}
				nextThread = nextThread->nextHilo;
			}
			else
			{
				nextThread = nextThread->nextHilo;
			}
		}
		return ++nextTicket;
	}
}


void giveTickets(HNCola pQueue, HN pThread, int pTickets)
{
	HN headThread = getHeadHN(pQueue);
	if(headThread != NULL)
	{
		HN nextThread = headThread->nextHilo;
		int previousTicket = pThread->finalTicket;
		pThread->finalTicket = pThread->finalTicket + pTickets;
		if(headThread->sorteo && headThread != pThread && headThread->initialTicket > previousTicket)
		{
			headThread->initialTicket = headThread->initialTicket + pTickets;
			headThread->finalTicket = headThread->finalTicket + pTickets;
		}
		while(nextThread != headThread)
		{
			if(nextThread->sorteo && nextThread != pThread)
			{
				if(nextThread->initialTicket > previousTicket)
				{
					nextThread->initialTicket = nextThread->initialTicket + pTickets;
					nextThread->finalTicket = nextThread->finalTicket + pTickets;
				}
				nextThread = nextThread->nextHilo;
			}
			else
			{
				nextThread = nextThread->nextHilo;
			}
		}
	}
}

void deleteTickets(HNCola pQueue, HN pThread, int pTickets)
{
	HN headThread = getHeadHN(pQueue);
	if(headThread != NULL)
	{
		HN nextThread = headThread->nextHilo;
		int previousTicket = pThread->finalTicket;
		if(pTickets > (pThread->finalTicket - pThread->initialTicket))
		{
			pTickets = pThread->finalTicket - pThread->initialTicket;
			pThread->finalTicket = pThread->initialTicket;
		}
		else
		{
			pThread->finalTicket = pThread->finalTicket - pTickets;
		}
		if(headThread->sorteo && headThread != pThread && headThread->initialTicket > previousTicket)
		{
			headThread->initialTicket = headThread->initialTicket - pTickets;
			headThread->finalTicket = headThread->finalTicket - pTickets;
		}
		while(nextThread != headThread)
		{
			if(nextThread->sorteo && nextThread != pThread)
			{
				if(nextThread->initialTicket > previousTicket)
				{
					nextThread->initialTicket = nextThread->initialTicket - pTickets;
					nextThread->finalTicket = nextThread->finalTicket - pTickets;
				}
				nextThread = nextThread->nextHilo;
			}
			else
			{
				nextThread = nextThread->nextHilo;
			}
		}
	}
}


void moveHeadToNextRoundRobin(HNCola pQueue, HN pThread)
{
	if(getHNRoundRobinCount(pQueue) > 1 && getHeadHN(pQueue) == pThread)
	{
		HN auxThred = pThread->nextHilo;
		while(auxThred->sorteo)
		{
			auxThred = auxThred->nextHilo;
		}
		setNewHead(pQueue, auxThred);
	}
}

void setNewHead(HNCola pQueue, HN pThread)
{
	HN headThread = getHeadHN(pQueue);
	while(headThread != pThread)
	{
		moveForward(pQueue);
		headThread = getHeadHN(pQueue);
	}
}

void restructureTickets(HNCola pQueue, HN pErasedThread)
{
	HN headThread = getHeadHN(pQueue);
	HN nextThread = headThread->nextHilo;
	int erasedTickets = (pErasedThread->finalTicket - pErasedThread->initialTicket) + 1;
	if(headThread != NULL)
	{
		if(headThread->sorteo && headThread->initialTicket > pErasedThread->finalTicket)
		{
			headThread->initialTicket = headThread->initialTicket - erasedTickets;
			headThread->finalTicket = headThread->finalTicket - erasedTickets;
		}
		while(nextThread != headThread)
		{
			if(nextThread->sorteo && nextThread->initialTicket > pErasedThread->finalTicket)
			{
				nextThread->initialTicket = nextThread->initialTicket - erasedTickets;
				nextThread->finalTicket = nextThread->finalTicket - erasedTickets;
				nextThread = nextThread->nextHilo;
			}
			else
			{
				nextThread = nextThread->nextHilo;
			}
		}
	}
}







//Funciones del sistema Mutex

int isMutexInQueue(HiloMutex *pMutex){

	if(mutexCola == NULL || pMutex == NULL){
		
		return 0;
	}
	else{
		
		MutexCola auxCola = mutexCola;
		
		while((auxCola != NULL) && (auxCola->mutex->lockNumber != pMutex->lockNumber)){
			
			
			auxCola = auxCola->nextMutex;
			
		}
		if(auxCola == NULL){
			return 0;
		}
		else{
			
			return 1;
		}
	}
}

MutexCola createNewMutexQueue(){
	
	MutexCola auxCola = (MutexCola)malloc(sizeof(struct MutexColaNodes));
	if(auxCola != NULL){
		
		auxCola->mutex = NULL;
		auxCola->nextMutex = NULL;
		auxCola->threadNodeList = NULL;
		return auxCola;
	}
	else{
		
		return NULL;
	}
}


HiloMutex* createNewMutexNode(){
	
	static long currentMutexValue = 0;
	HiloMutex* newMutexNode = (HiloMutex*)malloc(sizeof(struct MutexNode));
	newMutexNode->lockNumber = ++currentMutexValue;
	return newMutexNode;
}

void insertMutexQueue(MutexCola pMutex)
{
	if(pMutex != NULL)
	{
		pMutex->nextMutex = mutexCola;
		mutexCola = pMutex;
	}
}

MutexCola searchMutexQueue(HiloMutex *pMutex)
{
	if(mutexCola== NULL || pMutex == NULL)
	{
		return NULL;
	}
	else
	{
		MutexCola auxQueue = mutexCola;
		while((auxQueue != NULL)  && (auxQueue->mutex->lockNumber != pMutex->lockNumber))
		{
			auxQueue = auxQueue->nextMutex;
		}
		return auxQueue;
	}
}













