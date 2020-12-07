#ifndef LENGUAJE_H
#define LENGUAJE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

typedef struct archivoASCII
{
	int id;
	int segInicial;
	int segFinal;
	char* tipo;
	int posX;
	int posY;
	int ancho;
	int largo;
	char** caracteres;
	struct archivoASCII *siguiente;
}*Figura;

typedef struct datos
{
	int tiempoTotal;
	int largoCanvas;
	int anchoCanvas;
	int espacioOcupado;
	int segActual;
	Figura objetos;
}*Canvas;

Figura prepararFigura();
Canvas crearCanvas();
void imprimirObjeto(char** objeto, int largo);
void liberarFigura(Figura f);

#endif