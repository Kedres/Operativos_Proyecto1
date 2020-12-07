# Tecnológico de Costa Rica
## Escuela de Ingeniería en Computación
### Principios de sistemas operativos

# RememberRemember

### Desarrollado por
* #### Kevin Segura Rojas (2017153767)
* #### Carlos Mora Murillo (2017238926)
* #### Jorge Gatgens Villalobos (2017110109)

##### Alajuela, II-S 2020

******

## Introdución

Se recreará la biblioteca de pthread para entender cómo se puede hacer un subsistema dentro del mismo OS sin tocar el kernel principal. Además de esto se tendrá que hacer un programa que utilice la nueva biblioteca. Esta deberá tener las mismas funciones que la biblioteca original thread, pero además de estas tendrá una nueva llamada my_thread_chsched que se encargará de seleccionar el tipo de scheduling con que se ejecutara el hilo, entre los cuales tendremos RoundRobin, Sorteo y Tiempo Real. 

La nueva biblioteca deberá funcionar como una homóloga a la biblioteca original, es decir que se podrá compilar un proyecto que haya usado la biblioteca original
cambiandola por la creada en este proyecto.

Finalmente, el programa a crear para utilizar la nueva biblioteca será uno que muestre animaciones de caracteres ASCII en un canvas, esta será la pantalla donde se 
mostrarán las animaciones y podrá estar distribuida entre varios monitores.

Habrá un lenguaje en el programa que permitirá establecer las propiedades o parámetros de cada animación y el programa. La aplicación permitirá correrse en distintas 
PCs donde cada una se comunicará a través de sockets y estas aportarán su monitor para el despliegue de las animaciones.



## Ambiente de Desarrollo

SublimeText3,Geany: Mediante estos editores se realizó el código de todo el proyecto.

GCC: mediante esta herramienta compilamos el código de proyecto.

Ubuntu Linux: Sistema operativo en el que se desarrolló todo el proyecto.

GDB: Mediante esta herramienta depuramos nuestro código.

Github: Utilizamos este servicio como alojamiento de nuestro repositorio del proyecto.


## Estructuras de datos usadas y funciones

#### Listas circulares

#### Listas enlazadas simples

### Librerías utilizadas 

#### Ncurses.h
### UContext.h
### String.h


### Biblioteca mypthreads


#### my_thread_init

Esta función se encarga de iniciar un hilo que va a controlar todos los demás y además inicia toda la biblioteca.

#### my_thread_create

esta función se encarga de crear un hilo mas no realiza su ejecución, solamente le asigna su espacio y setea sus parámetros y contextos para su futura ejecución.

#### my_thread_yield
Con esta función se logra pausar la ejecución de un hilo.


#### my_thread_exit
Esta función se encarga de terminar la ejecución de un hilo sin embargo este sigue activo.

#### my_thread_chsched
Función que se encarga de cambiar el scheduler de un hilo durante su ejecución.

#### my_thread_sleep
Esta función se encarga de dormir la ejecución de un hilo por una cantidad de tiempo.

#### my_thread_cancel

Esta función se encarga de cancelar la ejecución de un hilo y dejarlo inutilizable.

#### my_thread_join

Esta función es la encargada de agregar hilos a la cola de ejecución de hilos y esperar a que termine la ejecución de los demás hilos.

#### my_thread_detach
Esta función hace que el hilo no retorna ningún valor al finalizar su ejecución. 

#### my_mutex_init
Esta función se encarga de inicializar las estructuras para el mutex.

#### my_mutex_lock

Esta función se encarga de bloquear la memoria compartida mediante un mutex.

#### my_mutex_unlock

Esta función se encarga de desbloquear la memoria compartida del mutex para el uso de los demás hilos.

#### realTime

Función que se encarga de llevar a cabo el scheduler de Real Time

#### SorteScheduler 

Esta es la función principal que ejecula el scheduler de sorteo.

#### RoundRobinScheduler

Función que ejecuta el scheduler round robin en los hilos.

### Biblioteca Animación

#### AnimarFigura e imprimirFigura

Funciones que llevan a cabo el animar y mostrar la objeto en el canvas.

#### MoverFigura

Función que mueve la animación en el canvas.

#### PrepararFigura

Función que carga la figura seleccionada en memoria con los parametros elegidos para esta.

## Instrucciones para ejecutar el programa

#### Para probar los hilos

gcc  -Wall pruebaHilos.c Schedulers.c mypthreads.c DataStructures.c -o prueba

./prueba

#### Para probar la animación

gcc -Wall Animacion.c Lenguaje.c -o Animacion -lncurses

./Animacion

## Actividades realizadas por estudiante

#### Carlos Mora M

|                                  Actividad                                      |    Fecha   | Tiempo     |
|:-------------------------------------------------------------------------------:|:----------:|------------|
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 5 horas    |
| Se realizo lectura sobre manejo de hilos en C                                   | 13/11/2020 | 5 horas    |
| Se realizo lectura e investigación sobre la implementación de hilos en C        | 18/11/2020 | 6 horas    |
| Se realizaron pruebas de manejo de funcionalidad de u_context en C              | 25/11/2020 | 5 horas    |
| Se realizo el primer intento de implementación de hilos en C                    | 26/11/2020 | 4 horas    |
| Se hizo la primer implementación de manejo de hilos en C medainamente funcional | 27/11/2020 | 3 horas    |
| Se realizó más investigación sobre la implementación de hilos en C              | 30/11/2020 | 3 horas    |
| Se creo el primer prototipo de hilos                                            | 03/12/2020 | 5 horas    |
| Se creo un versión más pulida de los hilos                                      | 04/12/2020 | 3 horas    |
| Se investigo sobre el mutex y animaciones                                       | 04/12/2020 | 3 horas    |
| Se implemento el mutex                                                          | 05/12/2020 | 3 horas    |
| Se realizaron las pruebas de funcionamiento                                     | 06/12/2020 | 2 horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 3 horas    |

Horas Totales: 50 horas.


#### Kevin Segura

|                                  Actividad                                      |    Fecha   | Tiempo     |
|:-------------------------------------------------------------------------------:|:----------:|------------|
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 4 horas    |
| Se realizo lectura sobre manejo de Scheduers en C                               | 12/11/2020 | 4 horas    |
| Se realizo lectura e investigación sobre la implementación de schedulers en C   | 15/11/2020 | 4 horas    |
| Se realizaron pruebas de manejo de funcionalidad de u_context en C              | 20/11/2020 | 5 horas    |
| Se realizo el primer intento de implementación de scheduler en C                | 25/11/2020 | 4 horas    |
| Se hizo la primer implementación de manejo de hilos en C medianamente funcional | 26/11/2020 | 2 horas    |
| Se realizó más investigación sobre la implementación de hilos en para la        |                         | 
solución de errores encontrados                                                   | 28/11/2020 | 3 horas    |
| Se creo el primer prototipo de hilos                                            | 29/12/2020 | 5 horas    |
| Se creo un versión más pulida de los hilos                                      | 30/12/2020 | 1 horas    |
| Se investigo sobre el mutex                                                     | 01/12/2020 | 3 horas    |
| Se implemento el mutex                                                          | 01/12/2020 | 3 horas    |
| Se realizaron las pruebas de funcionamiento                                     | 01/12/2020 | 2 horas    |
| Se comenzo con el diseño de la animación y buscar arte ASCII                    | 02/12/2020 | 3 Horas    |
| Se realizaron funciones como la de cargar archivos y una animación básica.      | 03/12/2020 | 4 Horas    |
| Se comenzo a programas la funcionalidad de multiples Displays.                  | 04/12/2020 | 3 Horas    |
| Se detuvo el anterior trabajo para la investigación de errores encontrados.     | 05/12/2020 | 4 Horas    |
| Se continuo investigando sobre los errores.                                     | 06/12/2020 | 5 Horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 1 hora     |

Horas Totales: 60 horas.


#### Jorge Gatgens

|                                  Actividad                                      |    Fecha   | Tiempo     |
|:-------------------------------------------------------------------------------:|:----------:|------------|
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 5 Horas    |
| Se investigo y diseño las animaciones                                           | 03/12/2020 | 6 Horas    |
| Implementación de canvas                                                        | 04/12/2020 | 3 Horas    |
| Implementación del menú de las animaciones junto con sus restricciones          | 04/12/2020 | 3 Horas    |
| Investigaciones sobre errores encontrados (Malloc principalmente)               | 05/12/2020 | 5 Horas    |
| Se realizaron las pruebas de funcionamiento                                     | 06/12/2020 | 2 Horas    |
| Se siguió investigando posibles soluciones para los errores                     | 06/12/2020 | 4 Horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 3 Horas    |

Horas Totales: 31 horas.

## Evaluación

## Autoevaluación

MyPthread:

- Scheduler RoundRobin: 5% de 5%
- Scheduler Sorteo: 5 % de 5%
- Scheduler en Tiempo Real: 5 % de 5%
- Cambio de Scheduler: 5 % de 5%
- Funciones de la biblioteca pthreads: 10 % de 10%

Documentación utilizando Markdown o Latex-PDF: 20 % de 20%
Diseño de lenguaje: 10% de 10%
Implementación de la animación: 8% de 20 %
Funcionamiento en Multiples Displays: 0% de 20 %
Extra: 0% de 10 %
Kick-off: 5 % de 5%

#### Carlos Mora
[1] [2] [3] [4] [5] Aprendizaje de Round Robin.[4]

[1] [2] [3] [4] [5] Aprendizaje de Tiempo Real.[4]

[1] [2] [3] [4] [5] Aprendizaje de Cambio de contexto.[5]

[1] [2] [3] [4] [5] Aprendizaje de Sorteo.[5]

#### Kevin Segura 
[1] [2] [3] [4] [5] Aprendizaje de Round Robin.[5]

[1] [2] [3] [4] [5] Aprendizaje de Tiempo Real.[5]

[1] [2] [3] [4] [5] Aprendizaje de Cambio de contexto.[4]

[1] [2] [3] [4] [5] Aprendizaje de Sorteo.[5]


#### Jorge Gatgens
[1] [2] [3] [4] [5] Aprendizaje de Round Robin. [4]

[1] [2] [3] [4] [5] Aprendizaje de Tiempo Real.[3]

[1] [2] [3] [4] [5] Aprendizaje de Cambio de contexto. [2]

[1] [2] [3] [4] [5] Aprendizaje de Sorteo. [4]


## Estado Final del proyecto

- Se logró la implementación completa de los hilos con sus schedulers.
- se logró implementar las animaciones a un nivel básico sin hilos.
- Además se desarrollo el lenguaje que se suponía que utilizaría la animación.

## Problemas encontrados y limitaciones
- Como principal limitanción tuvimos el desconocimiento del lenguaje.
- Problemas con el manejo de Ucontext en los hilos.
- Manejor de menoria con los hilos fue un problema especialmente en sus listas de hilos.
- Se encontraron varios problemas con la reserva de memoria para poder implementar varias animaciones.
- Problemas con mostrar animaciones con la librería Ncurses.
- Problema con poder abrir más de un archivo o hasta el mismo.
- Varios problemas de diseño porque planeabamos una cosa pero a la hora de pasarlo a la práctica no resulto viable.

## Reporte de Commits

@Kedres
Kedres committed 44 minutes ago

Update Documencion.md

@Kedres
Kedres committed 1 hour ago

Update Documencion.md

@Kedres
Kedres committed 1 hour ago

Merge branch 'main' of https://github.com/Kedres/Operativos_Proyecto1 … …

@gatgens
gatgens committed 1 hour ago
 
Animaciones completas

@gatgens
gatgens committed 1 hour ago
 
Update Documencion.md

@carlosmora179
carlosmora179 committed 1 hour ago

Create Documencion.md

@Kedres
Kedres committed 2 hours ago

Agregando canvas

@gatgens
gatgens committed 7 hours ago
 
Animación basica funcionando

@Kedres
Kedres committed 11 hours ago
 
primeras implementaciones de la animacion

@Kedres
Kedres committed 18 hours ago
 
Commits on Dec 5, 2020
se soluciono el problema con los hilos

@carlosmora179
carlosmora179 committed yesterday
 
Corrección en los Schedulers

@Kedres
Kedres committed yesterday
 
Pequeño arreglo al scheduler RoundRobin

@Kedres
Kedres committed yesterday
 
subiendo el archivo de schedulers con todos los Schedulers en su prim… …

@Kedres
Kedres committed yesterday
 
prueba de los hilos

@carlosmora179
carlosmora179 committed 2 days ago
 
Commits on Dec 4, 2020
pequeño cambio para agregar el include de los schedulers

@Kedres
Kedres committed 3 days ago
 
Primera implementación del Scheduler de tiempo real y el de sorteo

@Kedres
Kedres committed 3 days ago
 
Commits on Dec 3, 2020
Archivos de Hilos completos.

@carlosmora179
carlosmora179 committed 4 days ago
 
Commits on Nov 25, 2020
Initial commit

@Kedres
Kedres committed 11 days ago

## Lecciones Aprendidas

- Se aprendió bastante mediante la practica el uso de memoria en C, aunque no fue suficiente.
- También se aprendio el uso interno de los hilos y su funcionalidad aproximada en código al hacer nuestra propia.
- Investigar bastante por todos lados no centrase solo en generar con código.
- Trabajar lo más pronto posible para no estár contra reloj al final.
- No perder la calma ante los errores, y aunque no se logré completar la tarea, siempre algo valioso se aprende para el futuro.


## Bibliografía

Droid, L. I. (2015). Machinary. Obtenido de Machinary:
https://machiry.wordpress.com/2012/01/02/mfiber-simple-user-land-threadlibrary/
McGraw-Hill, E. (2008). MailxMail. Obtenido de MailxMail:
http://www.mailxmail.com/curso-informatica-procesos/procesos-c-funcion-sigprocmask
Perepelitsa, C. (2013). Quora. Obtenido de Quora: https://www.quora.com/Howdo-you-write-a-C-program-to-split-a-string-by-a-delimiter
Pina, C.(2004). MANUAL DE INICIACIÓN A LA LIBRERÍA NCURSES. Recuperado de: http://index-of.co.uk/Programming/ncurses.pdf
ASCII Art Archive.(s.f). Recuperado de: https://www.asciiart.eu/
