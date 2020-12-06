#include "Animacion.h"
#include "Lenguaje.h"

Canvas canva;
static int idFigura = 0;

void imprimirAnimacion(Figura f);
void moverFigura(Figura f);
void verificar(Figura f);
void insertarFigura(Figura f);

void animarFigura(Figura f)
{
	refresh();
	moverFigura(figura);
	liberarFigura(figura);
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
		f->posY++;
	}
}

void insertarFigura(Figura f) 
{
	f->id = ++idFigura;

	Figura nodo* = canva->objetos;

	while (nodo->siguiente != NULL) 
	{
		nodo = nodo->siguiente;
	}

	nodo->siguiente = f;

	f->siguiente = NULL;
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

			insertarFigura(figura);
		}
		else 
		{
			break;
		}
	}

	endwin();
	return 0;
}