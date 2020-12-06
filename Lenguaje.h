#ifndef LENGUAJE_H
#define LENGUAJE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct archivoASCII
{
	int segInicial;
	int segFinal;
	int segActual;
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
	Figura objetos;
}*DatosAnimacion;

Figura prepararFigura();
void imprimirObjeto(char** objeto, int largo);
void liberarFigura(Figura f);

#endif