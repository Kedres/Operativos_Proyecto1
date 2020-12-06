#include "DataStructures.h"
#include "mypthreads.h"
#include "Schedulers.h"

extern sigset_t sigProcMask;
extern HNCola threadsQueue;
extern struct itimerval timeQuantum;
extern long timeInterval;

int roundRobinControl = 0;
int sorteoControl = 0;
int tiquete = 0;

void printQueue(HNCola pQueue);

//--------------------------------------RoundRobin-----------------------------------------

//Funcion que retorna el primer hilo de la lista que rea del scheduler RoundRobin
static HN obtenerPrimeroRoundRobin(HNCola threadsQueue)
{
	//Moviendo al siguiente en la lista
	//printf("\n id %ld \n",getHeadHN(threadsQueue)->hiloID);
	if(getHeadHN(threadsQueue)->HilosCompleted == 0)
	{
		moveForward(threadsQueue);
	}

	HN hiloResultado = NULL;
	int i = 0;
	int totalHilos = getHNCount(threadsQueue);
	int hilosRoundRobin = getHNRoundRobinCount(threadsQueue);
	HN hiloHeader = getHeadHN(threadsQueue);

	while(i < totalHilos && hilosRoundRobin > 0)
	{
		//Revisa si el hilo es administrado por RoundRobin y si tiene que ejecutar algo
		//Sino se elimana de paso
		if (hiloHeader->HilosCompleted && hiloHeader->roundRobin)
		{
			if (threadsQueue->currentThread == hiloHeader)
			{
				threadsQueue->currentThread = NULL;
			}
			deleteHeadThread(threadsQueue);
			hiloHeader = getHeadHN(threadsQueue);
		//Revisa si el hilo no se puede administrar por el Scheduler por una u otra razón
		}else if (hiloHeader->HilosBlocked || hiloHeader->sorteo)
		{
			moveForward(threadsQueue);
			hiloHeader = getHeadHN(threadsQueue);
			i++;
		//No hay problema con el hilo revisado
		}else{
			hiloResultado = hiloHeader;
			i = totalHilos;
		}
	}
	return hiloResultado;
}

void roundRobinScheduler()
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	HN hiloActual = obtenerPrimeroRoundRobin(threadsQueue);
	HN hiloAnterior = threadsQueue->currentThread;
	//DeadLock
	if (hiloAnterior == NULL && hiloActual == NULL)
	{
		exit(1);
	//No hay RoundRobin para ser usado
	}else if (hiloActual == NULL)
	{
		threadsQueue->currentThreadCopy = hiloAnterior;
		threadsQueue->currentThread = NULL;
		sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		my_thread_yield();
	//En la selección anterior no se escogio ningún hilo 
	}else if(hiloAnterior == NULL && hiloActual != threadsQueue->currentThreadCopy)
	{
		threadsQueue->currentThread = hiloActual;
		timeQuantum.it_value.tv_usec = timeInterval;
		if (threadsQueue->currentThreadCopy != NULL)
		{
			HN aux = threadsQueue->currentThreadCopy;
			threadsQueue->currentThreadCopy = NULL;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			swapcontext(&(aux->hiloContext), &(hiloActual->hiloContext));
		}else
		{
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			setcontext(&(hiloActual->hiloContext));
		}
	//Todo bien
	}else
	{
		if (hiloAnterior != NULL && hiloAnterior != hiloActual)
		{
			threadsQueue->currentThread = hiloActual;
			threadsQueue->currentThreadCopy = NULL;
			timeQuantum.it_value.tv_usec = timeInterval;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			swapcontext(&(hiloAnterior->hiloContext), &(hiloActual->hiloContext));
		}else
		{
			threadsQueue->currentThread = hiloActual;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		}
	}
}


//---------------------------------------------Sorteo--------------------------------------

static void insertarValorHilosGenerados(int* hilosGenerados, int idHilo, int cantSorteo);
static void borrarHilosGenerados(int* hilosGenerados, int idHilo, int cantSorteo);
static int conteoHilosGenerados(int* hilosGenerados, int cantSorteo);
static int hilosBloqueados(HNCola cola);
static int existeEnHilos(int* hilosGenerados, int valorEncontradoHilos, int cantSorteo);
static int generarTiquete(HNCola cola, int* generarHilos, int cantSorteo);
static HN obtenerPrimeroSorteo(HNCola cola);



void sorteoScheduler()
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	HN hiloActual = obtenerPrimeroSorteo(threadsQueue);
	HN hiloAnterior = threadsQueue->currentThread;

	//Si no hay hilos para poner en funcionamiento desde hace un tiempo
	if(hiloAnterior == NULL && hiloActual == NULL)
	{
		//printf("Ocurrio un posible Deadlock en el schaduler sorteo.\n");
		exit(1);
	//No hay uno disponible para actuar
	}else if (hiloActual == NULL)
	{
		//printf("El siguiente hilo a funcionar esta en NULL.\n"); 
		threadsQueue->currentThreadCopy = threadsQueue->currentThread;
		threadsQueue->currentThread = NULL;
		sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		my_thread_yield();
	//RoundRobin se quedo sin hilos para funcionar por algún motivo o el anterior era del tipo sorteo y termino 
	}else if (hiloAnterior == NULL && hiloActual != threadsQueue->currentThreadCopy)
	{
		//printf("\naqui\n");
		threadsQueue->currentThread = hiloActual;
		timeQuantum.it_value.tv_usec = timeInterval;
		if(threadsQueue->currentThreadCopy != NULL)
		{
			//printf("El hilo anterior estaba en NULL, cambiandolo a otro posible\n");
			HN aux = threadsQueue->currentThreadCopy;
			threadsQueue->currentThreadCopy = NULL;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			swapcontext(&(aux->hiloContext), &(hiloActual->hiloContext));
			
		}else
		{
			//printf("El hilo anterior estaba en NULL, cambiandolo a otro posible\n");
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			setcontext(&(hiloActual->hiloContext));
		}
	}else{
		if (hiloAnterior != NULL && hiloAnterior != hiloActual)
		{
			//printf("Cambiando hilo %ld \n",threadsQueue->currentThread->hiloID);
			threadsQueue->currentThread = hiloActual;
			threadsQueue->currentThreadCopy = NULL;
			timeQuantum.it_value.tv_usec = timeInterval;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
			//printf("\naqui2\n");
			swapcontext(&(hiloAnterior->hiloContext), &(hiloActual->hiloContext));
			
		}else{
			threadsQueue->currentThread = hiloActual;
			sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
		}
	}
}

//Funcion que busca el primer hilo valido para el schedule de sorteo
//De paso se borran los que ya estan completos
static HN obtenerPrimeroSorteo(HNCola cola)
{
	HN hiloResultado = NULL;
	int cantSorteo = getHNSortCount(cola);
	int* generarHilos = (int*)calloc(cantSorteo, sizeof(int));
	tiquete = generarTiquete(cola, generarHilos, cantSorteo);	
	//printf("El tiquete inicial para el hilo fue: %i\n", tiquete);
	HN hiloHeader = searchThreadTicket(tiquete, cola);

	if (hilosBloqueados(cola))
	{
		//printf("Todos los hilos de sorteo bloqueados\n");
		tiquete = 0;
		hiloResultado = NULL;
	}else
	{
		int totalHilos = getHNSortCount(cola);
		while(conteoHilosGenerados(generarHilos, cantSorteo) <= totalHilos)
		{
			if (hiloHeader->HilosCompleted && hiloHeader->sorteo)
			{
				if (cola->currentThread == hiloHeader)
				{
					cola->currentThread = NULL;
				}
				borrarHilosGenerados(generarHilos, (int)hiloHeader->hiloID, cantSorteo);
				deleteSortThread(hiloHeader->hiloID, cola);
				if (getHNSortCount(cola) == 0 || hilosBloqueados(cola))
				{
					//printf("Imposible seguir administrando hilos sorteo en este momento\n");
					tiquete = 0;
					hiloResultado = NULL;
					break;
				}else
				{
					tiquete = generarTiquete(cola, generarHilos, cantSorteo);
					hiloHeader = searchThreadTicket(tiquete, cola);
					insertarValorHilosGenerados(generarHilos, (int)hiloHeader->hiloID, cantSorteo);

				}
			}else if (hiloHeader->HilosBlocked || hiloHeader->roundRobin)
			{
				//printf("Hilo bloqueado o de scheduler incompatible para sorteo.\n");
				tiquete = generarTiquete(cola, generarHilos, cantSorteo);
				hiloHeader = searchThreadTicket(tiquete, cola);
				insertarValorHilosGenerados(generarHilos, (int)hiloHeader->hiloID, cantSorteo);
			}else
			{
				//printf("Se encontro hilo optimo.\n");
				hiloResultado = hiloHeader;
				break;
			}
		}
	}
	free(generarHilos);
	return hiloResultado;
}

//Función que genera los tiquetes para los hilos
static int generarTiquete(HNCola cola, int* generarHilos, int cantSorteo)
{
	int random = ((rand() % (searchEndTicket(cola) -1)) + 1);

	while(existeEnHilos(generarHilos, (int)searchThreadTicket(random, cola)->hiloID, cantSorteo)){
		random = ((rand() % (searchEndTicket(cola) -1)) + 1);
	}
	return random;
}



//Función de validación para tiquetes únicos
static int existeEnHilos(int* hilosGenerados, int valorEncontradoHilos, int cantSorteo)
{
	int i = 0;
	int res = 0;
	while(i < cantSorteo && res == 0)
	{
		if(hilosGenerados[i] == valorEncontradoHilos)
		{
			res = 1;
		}else
		{
			i++;
		}
	}
	return res;
}

static int hilosBloqueados(HNCola cola)
{
	int res = 1;
	HN hiloHeader = getHeadHN(cola);
	HN hiloSiguiente = hiloHeader->nextHilo;
	if (hiloHeader->sorteo && !hiloHeader->HilosBlocked)
	{
		res = 0;
	}else
	{
		while(hiloHeader != hiloSiguiente && res == 1)
		{
			if (hiloSiguiente->sorteo && !hiloSiguiente->HilosBlocked)
			{
				res = 0;
			}else
			{
				hiloSiguiente = hiloSiguiente->nextHilo;
			}
		}
	}
	return res;
}

//Cuenta cuantos hilos han sido asignados hasta el momento
static int conteoHilosGenerados(int* hilosGenerados, int cantSorteo)
{
	int i = 0;
	int hilos = 0;

	while(i < cantSorteo)
	{
		if (hilosGenerados[i] != 0)
		{
			hilos++;
		}
		i++;
	}
	return hilos;
}

//Funcion que descarta el hilo del arreglo de hilos generados
static void borrarHilosGenerados(int* hilosGenerados, int idHilo, int cantSorteo)
{
	int i = 0;
	int encontrado = 0;
	while(i < cantSorteo && encontrado == 0)
	{
		if(hilosGenerados[i] == idHilo)
		{
			hilosGenerados[i] = 0;
		}
		i++;
	}
}

//Funcio que inserta
static void insertarValorHilosGenerados(int* hilosGenerados, int idHilo, int cantSorteo)
{
	int i = 0;
	while(i < cantSorteo)
	{
		if (hilosGenerados[i] == 0 && !existeEnHilos(hilosGenerados, idHilo, cantSorteo))
		{
			hilosGenerados[i] = idHilo;
			i = cantSorteo;
		}
		i++;
	}
}

//--------------------------------------------------RealTime---------------------------------------------

static void revisarHilos();
static void comprobarHilo(HN hilo);
static void aumentarPrioridad(HN hilo);
static void cambiarScheduler();

//Función que ejecuta el realTime Schaduler
void realTime()
{
	//printf("entre al scheduler\n");
	//printQueue(threadsQueue);
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	threadsQueue->quantums++;
	revisarHilos();
	cambiarScheduler();
	//printf("\n sali\n");
	//sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
}

//Revisa todos los hilos uno por uno por si ubiera que aumentar la prioridad de alguno
static void revisarHilos()
{
	HN hiloHeader = getHeadHN(threadsQueue);
	HN hiloSiguiente = hiloHeader->nextHilo;
	if (hiloHeader != NULL)
	{
		comprobarHilo(hiloHeader);
		while(hiloHeader != hiloSiguiente)
		{
			comprobarHilo(hiloSiguiente);
			hiloSiguiente = hiloSiguiente->nextHilo;
		}
	}
}

//Función que comprueba si es requerido cambiar la prioridad de un hilo
//Si estaba en RoundRobin lo cambia a Sorteo
static void comprobarHilo(HN hilo)
{
	if (hilo->limitTime > 0 && hilo->warningLevel > 0 && hilo->HilosCompleted == 0)
	{
		//De roundRobin a sorteo 
		if (hilo->roundRobin)
		{
			my_thread_chsched(hilo);
		}
		aumentarPrioridad(hilo);
	}
	//printf("estoy dentro\n");
}

//Da una cantidad de hilos acorde al warning solicitado
static void aumentarPrioridad(HN hilo)
{
	if(hilo->warningLevel == 1)
	{
		giveTickets(threadsQueue, hilo, 10);
	}
	else if(hilo->warningLevel == 2)
	{
		giveTickets(threadsQueue, hilo, 20);
	}
	else if(hilo->warningLevel == 3)
	{
		giveTickets(threadsQueue, hilo, 30);
	}
	hilo->ultimateWarningLevel = hilo->warningLevel;
	hilo->warningLevel = 0;
}

//Funcion que cambia el scheduler 
static void cambiarScheduler()
{
	//printf("Estoy en el cambio\n");
	int cantSorteo = getHNSortCount(threadsQueue);
	int cantRoundRobin = getHNRoundRobinCount(threadsQueue);
	if (sorteoControl && cantSorteo > 0 && cantRoundRobin == 0)
	{
		//printf("Primer caso de Sorteo\n");
		roundRobinControl = 0;
		sorteoScheduler();
	}else if (roundRobinControl && cantRoundRobin > 0 && cantSorteo == 0)
	{
		//printf("Primer caso de roundRobin\n");
		sorteoControl = 0;
		roundRobinScheduler();
	}else if (roundRobinControl && cantSorteo > 0)
	{
		//printf("segundo caso de sorteo\n");
		roundRobinControl = 0;
		sorteoControl = 1;
		sorteoScheduler();
	}else if (sorteoControl && cantRoundRobin > 0)
	{
		//printf("segundo caso de roundRobin\n");
		roundRobinControl = 1;
		sorteoControl = 0;
		roundRobinScheduler();
	}else
	{
		//printf("No hay threads en ninguno de los schedules\n");
		exit(0);
	}
}

void printQueue(HNCola pQueue)
{
    HN headThread = getHeadHN(pQueue);
    HN nextThread = headThread->nextHilo;
    if(headThread != NULL)
    {
        printf("Imprimiendo cola\n");
        printf("Head: %d\n", (int)headThread->hiloID);
        printf("Parent: %d\n", (int)pQueue->headParent->hiloID);
        printf("Thread: %d\n", (int)headThread->hiloID);
        
        while(headThread != nextThread)
        {
            printf("nextThread: %d\n", (int)nextThread->hiloID);
            nextThread = nextThread->nextHilo;
        }
        printf("Termino la impresion de la cola\n");
        
        printf("nextThread: %d\n", (int)nextThread->hiloID);
    }
}




//Con lo que probaba yo, descomentar si la necesita
/*
int main(int argc, char const *argv[])
{
	threadsQueue = createHNCola();
	HN HNMain = createNewHN();
	insertThread(threadsQueue, HNMain);
	realTime();
	freeThread(HNMain);
	free(threadsQueue);
	return 0;
}
*/
