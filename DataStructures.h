/*
 * DataStructures.h
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


#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define STACKSIZE 1024*8

typedef long thread;

typedef struct HiloNodes
{
    thread hiloID;
    ucontext_t hiloContext;
    struct HiloNodes *nextHilo;
    struct ListaHilosEspera *hilosEspera;
    long startQuantum;
    int HilosCompleted;
    int HilosBlocked;
    int HiloWaiting;
    int roundRobin;
    int sorteo;
    int initialTicket;
    int finalTicket;
    int warningLevel;
    int ultimateWarningLevel;
    int limitTime;
    int detach;
}*HN;

typedef struct ListaHilosEspera
{
    HN waitingThreadNode;
    struct ListaHilosEspera *nextHiloEspera;
}*HilosEspera;

typedef struct HNColas
{
    HN head;
    HN headParent;
    HN currentThread;
    HN currentThreadCopy;
    long countRoundRobin;
    long countSorteo;
    long count;
    long quantum;
    long quantums;
}*HNCola;

typedef struct HilosMuertosNodes
{
    thread threadID;
    void **returnValue;
    int HilosEsperando;
    struct HilosMuertosNodes *nextHiloMuertoNode;
}*HiloMuertoNode;


typedef struct HiloMuertoNodeCola
{
    HiloMuertoNode head;
    long count;
}*HilosMuertosNodesCola;

typedef struct MutexNode
{
   long lockNumber;
} HiloMutex;

typedef struct MutexColaNodes
{
    HiloMutex *mutex;
    struct MutexColaNodes *nextMutex;
    HilosEspera threadNodeList;
} *MutexCola;


//Hilos

HN getHeadHN(HNCola pQueue); //Obtiene el thread principal de la cola, el thread que se encuentra en la cabeza de la cola.
HN getNextHN(HNCola pQueue); //Obtiene el thread que le sigue a la cabeza.
HN createNewHN(); //Crea un nuevo nodo para un thread.
HN searchThread(thread pThreadID,HNCola pQueue); //Retorna un thread buscando por su ID en la cola.
HN searchThreadTicket(int pTicket, HNCola pQueue); //Retorna un thread buscando por su ID de tickete.
HNCola createHNCola(); //Instancia una nueva cola donde se guardaran los nodos que contienen los threads.
HilosEspera createWaitingThreadsList(); //Inicializa una nueva lista donde se guardaran los threds en espera de que otro finalice.
HiloMuertoNode searchDeadThread(HilosMuertosNodesCola pQueue, thread pThreadID); //Obtiene un thread dado su threadID de la lista de threads muertos.
HiloMuertoNode createNewDeadThreadNode(); //Instancia un nuevo nodo donde se guardara un thread que ya haya finalizado.
HilosMuertosNodesCola createDeadTheadsNodesQueue(); //Instancia una nueva cola donde se guardaran los threads que ya han terminado.
int moveForward(HNCola pQueue); //Avanza en la estructura, es decir, cambia la cabeza al siguiente y pone en el padre de la cabeza el thread anterior a la cabeza actual en la cola.
int getHNCount(HNCola pQueue); //Retorna la cantidad de threads que hay en la cola.
int getHNRoundRobinCount(HNCola pQueue); //Retorna la cantidad de threads adaministrados por el scheduler RoundRobin que hay en la cola.
int getHNSortCount(HNCola pQueue); //Retorna la cantidad de threads administrador por el scheduler Sort que hay en la cola.
int deleteHeadThread(HNCola pQueue); //Elimina el thread cabeza en la cola.
int deleteSortThread(thread pThreadID, HNCola pQueue); //Elimina el thread dado de la cola.
int insertThread(HNCola pQueue, HN pThread); //Agrega un nuevo thread al final de la cola (Que a su vez es una lista circular).
int insertWaitingThread(HN pTargetThread,HN pThreadAdded); //Agrega un nuevo thread a la lista de threads en espera (threads que hicieron un join para esperar la finalizacion de otro thread).
int insertDeadThred(HilosMuertosNodesCola pQueue, HiloMuertoNode pThread);// Agrega un nuevo thread al inicio de la cola y setea la caebeza al nuevo thread agregado.
int searchEndTicket(HNCola pQueue); //Busca el ultimo ticket que fue asignado a un proceso y lo devuelve sumandole uno.
void deleteDeadThread(HilosMuertosNodesCola pQueue, thread pThreadID); //Borra un deadThread de la lista.
void freeThread(HN pThread); //Libera el espacio asociado a un thread en la cola.
void giveTickets(HNCola pQueue, HN pThread, int pTickets); //Da mas ticketes al thread dado con la cantidad de tiquetes dados.
void deleteTickets(HNCola pQueue, HN pThread, int pTickets); //Borra ticketes al thread dado la cantidad enviada.
void setNewHead(HNCola pQueue, HN pThread); //Setea la cabeza al nuevo thread dado.
void restructureTickets(HNCola pQueue, HN pErasedThread); //Reestructura por completo los ticketes de los threads administrados por el scheduler sort.
void moveHeadToNextRoundRobin(HNCola pQueue, HN pThread); //Mueve la cabeza al siguiente thread administrado por un scheduler RoundRobin.

//Mutex

MutexCola searchMutexQueue(HiloMutex *pMutex);
MutexCola createNewMutexQueue();
HiloMutex *createNewMutexNode();
void insertMutexQueue(MutexCola pMutex);
int isMutexInQueue(HiloMutex *pMutex);

#endif
