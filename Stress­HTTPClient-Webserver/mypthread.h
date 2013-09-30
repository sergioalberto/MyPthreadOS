/* ------------------------------------------------------- */
/* My_Pthreads                                             */
/* Implemantacion de la Biblioteca Pthreads                */
/*                                                         */
/* El código que se tomó de base es de caracter libre,     */
/* desarrollado por Ching-Kuang Shene el 31 de agosto, 1998*/
/*                                                         */
/* ------------------------------------------------------- */

#include  <stdio.h>
#include  <stdlib.h>
#include  <setjmp.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

/* ------------------------------------------------------- */
/*Valores estáticos utilizados en la ejecución del programa*/
/* ------------------------------------------------------- */
#define MAX_TICKETS 100

/* ------------------------------------------------------- */
/*                  Estructuras de Datos                   */
/* ------------------------------------------------------- */

typedef struct PCB_NODE  *PCB_ptr; /* puntero al PCB       */

typedef struct PCB_NODE {          /* un PCB:              */
     jmp_buf   Environment;        /*   jump buffer        */
     int       Name;               /*   nombre del thread  */
     PCB_ptr   Next;               /*   próxima PCB        */
     int       Estado;             /* actividad del thread */
}  PCB;

typedef struct THREAD_NODE *thread_ptr;/*puntero a un thread*/

typedef struct THREAD_NODE {       /* un thread:           */
     PCB_ptr   my_pcb;             /*   puntero a su PCB   */
     thread_ptr Next;              /*   proximo thread     */
     int       Name;               /*   nombre del thread  */
} my_pthread;

typedef struct MUTEX {             /* un mutex:            */
    int        lock;               /*   estado del mutex   */
    thread_ptr owner;              /*   thread poseedor    */
} my_mutex;

/* ------------------------------------------------------- */
/*                     Datos Externos                      */
/* ------------------------------------------------------- */

jmp_buf   MAIN;                    /* jump buffer al  main */
jmp_buf   SCHEDULER;               /* para el scheduler    */

PCB_ptr   Head;                    /* inicio de los PCBs   */
PCB_ptr   Current;                 /* PCB actual           */
PCB_ptr   work;                    /* variable de trabajo  */

thread_ptr Head_T;                 /* inicio de los threads*/
thread_ptr Current_T;              /* thread actual        */
thread_ptr work_T;                 /* variable de trabajo  */

/* ------------------------------------------------------- */
/*         Variables utilizadas por el Scheduler           */
/* ------------------------------------------------------- */
int contador_t;
int non_match_up;
int join;

/* ------------------------------------------------------- */
/*                  Macro MY_THREAD_CREATE                 */
/*   Este macro crea un thread.                            */
/* ------------------------------------------------------- */

#define   MY_THREAD_CREATE(function, name) {                \
                    if (setjmp(MAIN) == 0)                  \
                    {                                       \
                         if(contador_t<MAX_TICKETS)         \
                         {                                  \
                            contador_t++;                   \
                            MY_THREAD_INIT(name);           \
                            (function)(name);               \
                         }                                  \
                         else                               \
                            printf("Maximo de tickets\n");  \
                    }                                       \
               }

                    
/* ------------------------------------------------------- */
/*                 Macro MY_THREAD_INIT                    */
/*   Este macro aloja el nuevo thread y su respectiva PCB  */
/*   y los introduce en la cadena de scheduling, una lista */
/*   circular.                                             */
/* ------------------------------------------------------- */

#define   MY_THREAD_INIT(name)     {                        \
                    work = (PCB_ptr) malloc(sizeof(PCB));   \
           work_T = (thread_ptr) malloc(sizeof(my_pthread));\
                    work->Name = name;                      \
                    work_T->Name = name;                    \
                    if (Head == NULL)                       \
                    {                                       \
                         Head = work;                       \
                         Head_T = work_T;                   \
                    }                                       \
                    else                                    \
                    {                                       \
                         Current->Next = work;              \
                         Current_T->Next = work_T;          \
                    }                                       \
                    work->Next = Head;                      \
                    work_T->Next = Head_T;                  \
                    work->Estado = 0;                       \
                    Current = work;                         \
                    Current_T = work_T;                     \
                    Current_T->my_pcb = Current;            \
                    if (setjmp(work->Environment) == 0)     \
                         longjmp(MAIN, 1);                  \
               }

/* ------------------------------------------------------- */
/*                 Macro MY_THREAD_EXIT                    */
/*  Este macro coloca el thread actual en estado innactivo */
/* ------------------------------------------------------- */

#define    MY_THREAD_EXIT(name)    {                        \
                  Current->Estado = 1;                      \
                  MY_THREAD_YIELD(name);                    \
                                                            \
}
                         
/* ------------------------------------------------------- */
/*                 Macro MY_THREAD_YIELD                   */
/*   Este macro simular el paso del CPU de un thread a     */
/*   vía scheduler. El scheduler será quien seleccione     */
/*   cual será el siguiente hilo a ejecutarse.             */
/* ------------------------------------------------------- */

#define   MY_THREAD_YIELD(name)    {                        \
                    if (setjmp(Current->Environment) == 0)  \
                    longjmp(SCHEDULER, 1);                  \
               }

/* ------------------------------------------------------- */
/*                 Macro MY_THREAD_JOIN                    */
/*   Macro meramente ilustrativo. Su trabajo internamente  */
/*   es realizado por el Scheduler. Bloquea el thread      */
/*   actual hasta que el thread objetivo llegue a su final */
/* ------------------------------------------------------- */

#define   MY_THREAD_JOIN() {                                \
                     join = 1;                              \
               }                                            \

/* ------------------------------------------------------- */
/*                 Funtions                                */
/* ------------------------------------------------------- */

thread_ptr getThread(int name);
PCB_ptr MY_THREAD_getARG(thread_ptr thread);
void  MY_MUTEX_INIT(my_mutex* mutex);
void  MY_MUTEX_LOCK(my_mutex* mutex);
void MY_MUTEX_UNLOCK(my_mutex* mutex);
int MY_MUTEX_TRYLOCK(my_mutex* mutex);
void MY_MUTEX_DESTROY(my_mutex* mutex);
void Scheduler(void);
