#include "Animacion.h"
#include "Lenguaje.h"

void imprimirAnimacion(Figura f);

void animarFigura(Figura f)
{
	printf("elle\n");
	initscr();
	//move(10,2);
 	//printw("Hola Bulma!!!");
 	//refresh();
 	//getch();
 	//sleep(2);
 	//erase();
 	//move(11,2);
 	//printw("me movi");
 	//refresh();
	//imprimirAnimacion(f);
 	///refresh();
 	getch();
	endwin();
	printf("sali\n");
}

void imprimirAnimacion(Figura f)
{
	for (int i = 0; i < f->largo; ++i)
	{
		move((f->posX)+i, f->posY);
		printw(f->caracteres[i]);
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
	Figura figura = prepararFigura();
	animarFigura(NULL);
	liberarFigura(figura);
	return 0;
}