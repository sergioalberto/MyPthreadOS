#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define   MAX_COUNT   5
#define MAX_TICKETS 100

typedef unsigned int size_t;

#define EINVAL          22               // Invalid argument
#define PRIORITY_NORMAL          8
#define PTHREAD_CREATE_JOINABLE       0
#define PTHREAD_EXPLICIT_SCHED        1
#define PTHREAD_SCOPE_SYSTEM          1

//int contador_t;
//int join;

int contador_t;
int non_match_up;
int join;

/* ------------------------------------------------------- */
/*                     Data Structures                     */
/* ------------------------------------------------------- */

typedef struct PCB_NODE  *PCB_ptr; /* pointer to a PCB     */

typedef struct PCB_NODE {          /* a PCB:               */
     jmp_buf   Environment;        /*   jump buffer        */
     int       Name;               /*   thread name: unused*/
     PCB_ptr   Next;               /*   next PCB           */
     int       Estado;
}  PCB;    

typedef struct THREAD_NODE *thread_ptr;

typedef struct THREAD_NODE {
     PCB_ptr   my_pcb;
     thread_ptr Next;
     int       Name;
} my_pthread;

/* ------------------------------------------------------- */
/*                      External Data                      */
/* ------------------------------------------------------- */

jmp_buf   MAIN;                    /* jump buffer for main */
jmp_buf   SCHEDULER;               /* for the scheduler    */

PCB_ptr   Head;                    /* head of the PCBs     */
PCB_ptr   Current;                 /* current executing    */
PCB_ptr   work;                    /* working variable     */

thread_ptr Head_T;
thread_ptr Current_T;
thread_ptr work_T;

/* ------------------------------------------------------- */
/*                    Macro THREAD_CREATE                  */
/*   This macro creates a thread.                          */
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
/*                 Macro THREAD_INIT                       */
/*   This macro allocates a PCB for a thread and puts it   */
/*   into the scheduling chain, which is a circular list.  */
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

#define    MY_THREAD_EXIT(name)    {                        \
                  Current->Estado = 1;                      \
                  MY_THREAD_YIELD(name);                    \
                                                            \
}
                         
/* ------------------------------------------------------- */
/*                  Macro THREAD_YIELD                     */
/*   This macro simulates yielding control by jumping      */
/*   into the scheduler.  The scheduler picks the next     */
/*   thread to run.                                        */
/* ------------------------------------------------------- */

#define   MY_THREAD_YIELD(name)    {                        \
                    if (setjmp(Current->Environment) == 0)  \
                    longjmp(SCHEDULER, 1);                  \
               }

#define   MY_THREAD_JOIN() {                                \
                     join = 1;                              \
               }                                            \


int pthread_attr_init(pthread_attr_t *attr);

thread_ptr getThread(int name);
PCB_ptr MY_THREAD_getARG(thread_ptr thread);
void Scheduler(void);

