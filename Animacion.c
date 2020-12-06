#include "Animacion.h"
#include "Lenguaje.h"

void imprimirAnimacion(Figura f);
void moverFigura(Figura f);

void animarFigura(Figura f)
{
	initscr();
	Figura figura = prepararFigura();
	//imprimirAnimacion(figura);
	refresh();
	moverFigura(figura);
	liberarFigura(figura);
 	getch();
	endwin();
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

int main(int argc, char const *argv[])
{
	/*
	system("clear");
	printf("Hola a todos\n");
	sleep(0.5);
	system("clear");
	*/
	/*initscr();
	move(10,2);
 	printw("Hola Bulma!!!");
 	refresh();
 	//getch();
 	sleep(2);
 	erase();
 	move(11,2);
 	printw("me movi");
 	refresh();
 	getch();
	endwin();*/
	//Figura figura = prepararFigura();
	animarFigura(NULL);
	//liberarFigura(figura);
	return 0;
}