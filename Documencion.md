# Tecnológico de Costa Rica
## Escuela de Ingeniería en Computación
### Principios de sistemas operativos

# Mi DDoS

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

## Instrucciones para ejecutar el programa


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
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 5 horas    |
| Se realizo lectura sobre manejo de Scheduers en C                               | 13/11/2020 | 5 horas    |
| Se realizo lectura e investigación sobre la implementación de schedulers en C   | 18/11/2020 | 6 horas    |
| Se realizaron pruebas de manejo de funcionalidad de u_context en C              | 25/11/2020 | 5 horas    |
| Se realizo el primer intento de implementación de scheduler en C                | 26/11/2020 | 4 horas    |
| Se hizo la primer implementación de manejo de hilos en C medainamente funcional | 27/11/2020 | 3 horas    |
| Se realizó más investigación sobre la implementación de hilos en C              | 30/11/2020 | 3 horas    |
| Se creo el primer prototipo de hilos                                            | 03/12/2020 | 5 horas    |
| Se creo un versión más pulida de los hilos                                      | 04/12/2020 | 3 horas    |
| Se investigo sobre el mutex y animaciones                                       | 04/12/2020 | 3 horas    |
| Se implemento el mutex                                                          | 05/12/2020 | 3 horas    |
| Se realizaron las pruebas de funcionamiento                                     | 06/12/2020 | 2 horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 3 horas    |

Horas Totales: 50 horas.


#### Jorge Gatgens

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

## Autoevaluación

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


## Lecciones Aprendidas

#### Carlos Mora

#### Kevin Segura

#### Jorge Gatgens



## Bibliografía


