#include "DataStructures.c"
#include "mypthreads.c"
#include "Schedulers.h"

sigset_t mascara;
HNCola threadsQueue;
struct itimerval timeQuantum;
long timeInterval;

int roundRobinControl = 0;
int sorteoControl = 0;
int tiquete = 0;

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
	sigprocmask(SIG_BLOCK, &mascara, NULL);
	HN hiloActual = obtenerPrimeroSorteo(threadsQueue);
	HN hiloAnterior = threadsQueue->currentThread;

	//Si no hay hilos para poner en funcionamiento desde hace un tiempo
	if(hiloAnterior == NULL && hiloActual == NULL)
	{
		printf("Ocurrio un posible Deadlock en el schaduler sorteo.\n");
		exit(1);
	//No hay uno disponible para actuar
	}else if (hiloActual == NULL)
	{
		printf("El siguiente hilo a funcionar esta en NULL.\n"); 
		threadsQueue->currentThreadCopy = threadsQueue->currentThread;
		threadsQueue->currentThread = NULL;
		my_thread_yield();
	//RoundRobin se quedo sin hilos para funcionar por algún motivo o el anterior era del tipo sorteo y termino 
	}else if (hiloAnterior == NULL && hiloActual != threadsQueue->currentThreadCopy)
	{
		threadsQueue->currentThread = hiloActual;
		timeQuantum.it_value.tv_usec = timeInterval;
		if(threadsQueue->currentThreadCopy != NULL)
		{
			printf("El hilo anterior estaba en NULL, cambiandolo a otro posible\n");
			HN aux = threadsQueue->currentThreadCopy;
			threadsQueue->currentThreadCopy = NULL;
			swapcontext(&(aux->hiloContext), &(hiloActual->hiloContext));
		}else
		{
			printf("El hilo anterior estaba en NULL, cambiandolo a otro posible\n");
			setcontext(&(hiloActual->hiloContext));
		}
	}else{
		if (hiloAnterior != NULL && hiloAnterior != hiloActual)
		{
			printf("Cambiando hilo\n");
			threadsQueue->currentThread = hiloActual;
			threadsQueue->currentThreadCopy = NULL;
			timeQuantum.it_value.tv_usec = timeInterval;
			swapcontext(&(hiloAnterior->hiloContext), &(hiloActual->hiloContext));
		}else{
			threadsQueue->currentThread = hiloActual;
		}
	}
	sigprocmask(SIG_UNBLOCK, &mascara, NULL);
}

//Funcion que busca el primer hilo valido para el schedule de sorteo
//De paso se borran los que ya estan completos
static HN obtenerPrimeroSorteo(HNCola cola)
{
	HN hiloResultado = NULL;
	int cantSorteo = getHNSortCount(threadsQueue);
	int* generarHilos = (int*)calloc(cantSorteo, sizeof(int));
	tiquete = generarTiquete(threadsQueue, generarHilos, cantSorteo);	
	printf("El tiquete inicial para el hilo fue: %i\n", tiquete);
	HN hiloHeader = searchThreadTicket(tiquete, threadsQueue);

	if (hilosBloqueados(cola))
	{
		printf("Todos los hilos de sorteo bloqueados\n");
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
					printf("Imposible seguir administrando hilos sorteo en este momento\n");
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
				printf("Hilo bloqueado o de scheduler incompatible para sorteo.\n");
				tiquete = generarTiquete(cola, generarHilos, cantSorteo);
				hiloHeader = searchThreadTicket(tiquete, cola);
				insertarValorHilosGenerados(generarHilos, (int)hiloHeader->hiloID, cantSorteo);
			}else
			{
				printf("Se encontro hilo optimo.\n");
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

	while(existeEnHilos(generarHilos, (int)searchThreadTicket(random, threadsQueue)->hiloID, cantSorteo)){
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
			hilosGenerados[i] == 0;
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
	printf("entre al scheduler\n");
	sigprocmask(SIG_BLOCK, &mascara, NULL);
	threadsQueue->quantums++;
	revisarHilos();
	//cambiarScheduler();
	sigprocmask(SIG_UNBLOCK, &mascara, NULL);
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
	printf("estoy dentro\n");
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
	printf("Estoy en el cambio\n");
	int cantSorteo = getHNSortCount(threadsQueue);
	int cantRoundRobin = getHNRoundRobinCount(threadsQueue);
	if (sorteoControl && cantSorteo > 0 && cantRoundRobin == 0)
	{
		printf("Primer caso de Sorteo\n");
		roundRobinControl = 0;
		sorteoScheduler();
	}else if (roundRobinControl && cantRoundRobin > 0 && cantSorteo == 0)
	{
		printf("Primer caso de roundRobin\n");
		sorteoControl = 0;
		//roundRobinScheduler();
	}else if (roundRobinControl && cantSorteo > 0)
	{
		printf("segundo caso de sorteo\n");
		roundRobinControl = 0;
		sorteoControl = 1;
		sorteoScheduler();
	}else if (sorteoControl && cantRoundRobin > 0)
	{
		printf("segundo caso de roundRobin\n");
		roundRobinControl = 1;
		sorteoControl = 0;
		//roundRobinScheduler();
	}else
	{
		printf("No hay threads en ninguno de los schedules\n");
		exit(0);
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