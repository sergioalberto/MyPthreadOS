/* ------------------------------------------------------- */
/* My_Pthreads                                             */
/* Implemantacion de la Biblioteca Pthreads                */
/*                                                         */
/* El código que se tomó de base es de caracter libre,     */
/* desarrollado por Ching-Kuang Shene el 31 de agosto, 1998*/
/*                                                         */
/* ------------------------------------------------------- */#

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "mypthread.h"

/* ------------------------------------------------------- */
/* FUNCION  getThread():                                   */
/* Encargada de realizar búsquedas por un thread especifico*/
/* según sea el numero identificador del mismo             */
/* ------------------------------------------------------- */

thread_ptr getThread(int name)
{
    int inicial = Current_T->Name;
    int first = 1;
    
    while(1)
    {
        if(Current_T->Name!=inicial || first)
        {
            first = 0;
            if(Current_T->Name==name)
            {
                return Current_T;
                break;
            }
            else
                Current_T = Current_T->Next;
        }
        else
            break;
    }
    return NULL;
}

/* ------------------------------------------------------- */
/* FUNCION  MY_THREAD_getARG():                            */
/* Encargada de retornar los atributos del PCB del thread  */
/* ------------------------------------------------------- */

PCB_ptr MY_THREAD_getARG(thread_ptr thread)
{
    if(thread != NULL)
        return thread->my_pcb;
    else
        return NULL;
}

/* ------------------------------------------------------- */
/* FUNCION  MY_MUTEX_INIT():                               */
/* Encargada inicializar en 0 el lock del mutex objetivo   */
/* ------------------------------------------------------- */

void  MY_MUTEX_INIT(my_mutex* mutex)
{
    mutex->lock=0;                       
}

/* ------------------------------------------------------- */
/* FUNCTION  MY_MUTEX_LOCK():                               */
/* Encargada setear en 1 el lock del mutex objetivo en caso*/
/* de que el valor del lock sea 0 y asignarle el mutex al  */  
/* thread que lo solicitó. Si el valor del lock es 1       */
/* inicialmente, el hilo cederá el CPU ya que no puede     */
/* entrar a la región crítica del programa.                */
/* ------------------------------------------------------- */

void  MY_MUTEX_LOCK(my_mutex* mutex)
{
    while(1)
    {
        if(mutex->lock==1)
        {
            longjmp(SCHEDULER, 1);
        }

        mutex->lock==1;
        mutex->owner=Current_T;
        break;
    }
}

/* ------------------------------------------------------- */
/* FUNCION  MY_MUTEX_UNLOCK():                             */
/* Encargada liberar el mutex objetivo del thread asignado */
/* y resetear su lock a un valor de 0                      */
/* ------------------------------------------------------- */

void MY_MUTEX_UNLOCK(my_mutex* mutex)
{
    mutex->lock=0;
    mutex->owner=NULL;
}

/* ------------------------------------------------------- */
/* FUNCION  MY_MUTEX_TRYLOCK():                            */
/* Retorna el valor del lock del mutex objetivo            */
/* ------------------------------------------------------- */

int MY_MUTEX_TRYLOCK(my_mutex* mutex)
{
    return mutex->lock;
}

/* ------------------------------------------------------- */
/* FUNCION  MY_MUTEX_DESTROY():                            */
/* Libera la memoria que estaba siendo utilizada por el    */
/* mutex objetivo                                          */
/* ------------------------------------------------------- */

void MY_MUTEX_DESTROY(my_mutex* mutex)
{
    free(mutex);
}

/* ------------------------------------------------------- */
/* FUNCION  Scheduler():                                   */
/* El Scheduler es primero llmado por el main del programa */
/* para setear un punto de inicio. Luego se reingresa a él */
/* a través del macro MY_THREAD_YIELD. En esta función se  */
/* realiza el paso de prioridad entre los hilos que tengan */
/* tiquetes para poder ser ejecutados.                     */
/* ------------------------------------------------------- */

void Scheduler(void)
{
     if (setjmp(SCHEDULER) == 0)
          longjmp(MAIN, 1);
     
     Current = Current->Next;
     if(Current->Estado==0)
     {
        non_match_up = 0;
        longjmp(Current->Environment, 1); 
     }
     else
     {
        non_match_up++;
        if(contador_t>non_match_up)
            longjmp(SCHEDULER, 1);
        else
        {
            MY_THREAD_JOIN();
        }
     }
}
