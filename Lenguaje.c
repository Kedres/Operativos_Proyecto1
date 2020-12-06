#include "Lenguaje.h"

Figura cargarFigura(char* nombre);
void limpiar (char *cadena);
void liberarCaracteres(char** objeto, int largo);
void rellenarFigura(Figura f);
void liberarFigura(Figura f);
void imprimirObjeto(char** objeto, int largo);

Figura prepararFigura()
{
	char* ingreso = (char *) malloc (50 * sizeof(char));
	//printf("Por favor ingerse el nombre de la figura: ");
	//fgets(ingreso, 100, stdin);
	//limpiar(ingreso);
	Figura objeto = cargarFigura(ingreso);
	if (objeto != NULL)
	{
		rellenarFigura(objeto);
		free(ingreso);
		//liberarFigura(objeto);
	}else{
		printf("Nombre de figura equivocado\n");
	}
	return objeto;
}

void limpiar (char *cadena)
{
  char *p;
  p = strchr (cadena, '\n');
  if (p)
    *p = '\0';
}

Figura cargarFigura(char* nombre)
{
	Figura res = NULL;

	char largo[3];
	char ancho[3];

	int parametros = 0;
	int contador = 0;
	int fila = 0;
	int anchoNum;
	int largoNum;
	char caracter;
	char **objeto;
	FILE *archivo;

	printw("Nombre de la figura: ");
	scanw("%s", nombre);
	//limpiar(nombre);
	archivo = fopen(nombre, "r");
	if(archivo == NULL)
	{
		//Manejo de errores
		printw("No se logro encontrar el archivo\n");
	}else
	{
		printw("archivo encontrado\n");
		while((caracter = fgetc(archivo)) != EOF)
		{
			if (parametros < 3)
			{
				//strcmp(caracter, ",") == 0
				if (caracter == ',')
				{
					parametros++;
					contador = 0;
				}else if (parametros == 0)
				{
					largo[contador] = caracter;
					contador++;
				}else if (parametros == 1)
				{
					ancho[contador] = caracter;
					contador++;
				}else
				{
					parametros++;
					largoNum = atoi(largo);
					anchoNum = atoi(ancho);
					contador = 0;
					objeto = (char **) malloc (largoNum * sizeof(char*));
					for (int i = 0; i < largoNum; ++i)
					{
						objeto[i] = (char *) malloc (anchoNum * sizeof(char));
					}
					fgetc(archivo);
				}
			}
			else if (caracter != '\n')
			{
				objeto[fila][contador] = caracter;
				contador++;
			}else{
				contador = 0;
				fila++;
			}
		}
		res = (Figura) malloc (sizeof(Figura));
		res->caracteres = objeto;
		res->ancho = anchoNum;
		res->largo = largoNum;
		fclose(archivo);
	}
	return res;
}

void rellenarFigura(Figura f)
{
	///printw("\n");
	int ingreso;
	printw("Ingrese el tiempo de inicio: ");
	scanw("%d", &ingreso);
	//limpiar(ingreso);
	f->segInicial = ingreso;
	//printw("%d", f->segActual);
	
	printw("Ingrese el tiempo de fin: ");
	scanw("%d",&ingreso);
	//limpiar(ingreso);
	f->segFinal = ingreso;

	//char* ingreso2 = (char *) malloc (50 * sizeof(char));
	printw("Posición X: ");
	scanw("%d", &ingreso);
	//limpiar(ingreso2);
	f->posX = ingreso;
	//free(ingreso2);

	printw("Posición Y: ");
	scanw("%d", &ingreso);
	//limpiar(ingreso);
	f->posY = ingreso;

	char* ingresoTipo = (char *) malloc (50 * sizeof(char));
	printw("Ingrese el tipo: ");
	scanw("%s", ingresoTipo);
	//limpiar(ingresoTipo);
	f->tipo = ingresoTipo;
	f->siguiente = NULL;
}

void imprimirObjeto(char** objeto, int largo)
{
	for (int i = 0; i < largo; ++i)
	{
		printf("%s\n", objeto[i]);
	}
}

void liberarFigura(Figura f)
{
	free(f->tipo);
	liberarCaracteres(f->caracteres, f->largo);
	//free(f);
}

void liberarCaracteres(char** objeto, int largo)
{
	for (int i = 0; i < largo; i++)
	{
		free(objeto[i]);
	}
	free(objeto);
}

/*int main(int argc, char const *argv[])
{
	prepararFigura();
	return 0;
}*/

