#include "Animacion.h"
#include "Lenguaje.h"
#include "mypthreads.h"

Canvas canva;
static int idFigura = 0;

void imprimirAnimacion(Figura f);
void moverFigura(Figura f);
void insertarFigura(Figura f);
int validarInsercionFigura(Figura f);
int validarMovimientoFigura(Figura f);
void empezarAnimacion();


void animarFigura(Figura f)
{
	refresh();
	moverFigura(f);
	liberarFigura(f);
 	getch();
}

void imprimirAnimacion(Figura f)
{
	int aux = f->posX;
	for (int i = 0; i < f->largo; ++i)
	{
		move(aux+i, f->posY);
		printw(f->caracteres[i]);
	}
}

void moverFigura(Figura f)
{
	for (int i = 0; i < f->segFinal; ++i)
	{
		erase();
		imprimirAnimacion(f);
		refresh();
		sleep(1);
		if (validarMovimientoFigura(f))
		{
			f->posY++;	
		} 
	}
}

void insertarFigura(Figura f) 
{
	
	f->id = ++idFigura;

	Figura nodo = canva->objetos;

	if (nodo != NULL) 
	{
		while (nodo->siguiente != NULL) 
		{
			nodo = nodo->siguiente;
		}

		nodo->siguiente = f;
	}
	else 
	{
		canva->objetos = f;
	}

	int sumarFigura = canva->espacioOcupado + (f->posY * f-> posX);

	canva->espacioOcupado = sumarFigura;

	f->siguiente = NULL;
}

void empezarAnimacion()
{
	Figura f = canva->objetos;

	while (f != NULL)
	{
		animarFigura(f);
		f = f->siguiente;
	}
}

int validarMovimientoFigura(Figura f)
{
	int validacion = 1;

	if ((f->posX+f->ancho) > canva->anchoCanvas || (f->posY+f->largo) > canva->largoCanvas) 
	{
		validacion = 0;
	}

	int ultimaFila;

	Figura actual = canva->objetos;
	
	while (actual != NULL)
	{
		if (actual->id != f->id)
		{
			ultimaFila = actual->posX + actual->ancho;

			if (actual->posX < f->posY && f->posY < ultimaFila)
			{
				if ((f->posX + f->ancho + 1) > actual->posY)
				{
					validacion = 0;
				}
			}
			else if (actual->posX < f->posY+f->largo && f->posY+f->largo < ultimaFila)
			{
				if ((f->posX + f->ancho + 1) > actual->posY)
				{
					validacion = 0;
				}
			}
		}

		actual = actual->siguiente;
	}

	return validacion;
}

int validarInsercionFigura(Figura f)
{
	int validacion = 1;

	if ((f->posX+f->ancho) > canva->anchoCanvas || (f->posY+f->largo) > canva->largoCanvas) 
	{
		validacion = 0;
	}

	int auxiliar = canva->espacioOcupado + (f->posX * f->posY);

	float espacioLibre = (canva->largoCanvas * canva->anchoCanvas) * 0.4f;

	if (((canva->largoCanvas * canva->anchoCanvas) - espacioLibre) < auxiliar)
	{
		validacion = 0;
	}

	return validacion;
}

int main(int argc, char const *argv[])
{
	initscr();

	int opcion;

	while (true)
	{
		erase();

		printw("1. Crear canvas\n");
		printw("2. Agregar figura\n");
		printw("3. Empezar animacion\n");
		printw("Ingrese su opcion: ");

		scanw("%d", &opcion);

		erase();

		if (opcion == 1) 
		{
			canva = crearCanvas();
		}
		else if (opcion == 2)
		{
			Figura figura = prepararFigura();

			if (validarInsercionFigura(figura))
			{
				insertarFigura(figura);	
			}
			else
			{
				printw("La figura que ha introducido sobrepasa los limites del canvas\n");
				printw("Presione una tecla para continuar...");
				liberarFigura(figura);
				getch();
			}
		}
		else 
		{
			if (canva->objetos != NULL)
			{
				empezarAnimacion();
			}
			else
			{
				erase();
				printw("Debe insertar la menos una animacion\n");
				printw("Presione una tecla para continuar...");
				getch();
			}
		}
	}

	endwin();
	return 0;
}