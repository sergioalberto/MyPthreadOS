/* ------------------------------------------------------- */
/* PROGRAM  mtp.c:                                         */
/*   This program shows a poorman's multithreaded kernel   */
/* using setjmp() and longjmp().                           */
/*                                                         */
/*                                       Ching-Kuang Shene */
/*                                         August 31, 1998 */
/* ------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define   MAX_COUNT   5
#define MAX_TICKETS 100

int counter1;
int counter2;
int counter3;
int counter4;
int contador_t = 0;
int non_match_up = 0;
int join = 0;

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

/* ------------------------------------------------------- */
/* FUNCTION  funct_1():                                    */
/*   A function to be run as a thread.                     */
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

PCB_ptr MY_THREAD_getARG(thread_ptr thread)
{
    if(thread != NULL)
        return thread->my_pcb;
    else
        return NULL;
}

void  funct_1(int  name)
{
    int  i;
     
    for(counter1 = 0;counter1 < MAX_COUNT-1;counter1++) {/* running the thread   */
         for (i = 1; i <= MAX_COUNT; i++)
              printf("funct_1() executing\n");
         MY_THREAD_YIELD(name);      /* yield control        */
    }
    MY_THREAD_EXIT(name);
}
                              
/* ------------------------------------------------------- */
/* FUNCTION  funct_2():                                    */
/*   A function to be run as a thread.                     */
/* ------------------------------------------------------- */

void  funct_2(int  name)
{
     int  i;
     
     for(counter2 = 0;counter2 < MAX_COUNT-1;counter2++) {
          for (i = 1; i <= MAX_COUNT; i++)
               printf("   funct_2() executing\n");
          MY_THREAD_YIELD(name);
     }
     MY_THREAD_EXIT(name);
}
                              
/* ------------------------------------------------------- */
/* FUNCTION  funct_3():                                    */
/*   A function to be run as a thread.                     */
/* ------------------------------------------------------- */

void  funct_3(int  name)
{
     int  i;
     
     for(counter3 = 0;counter3 < MAX_COUNT-1;counter3++) {
          for (i = 1; i <= MAX_COUNT; i++)
               printf("      funct_3() executing\n");
          MY_THREAD_YIELD(name);
     }
     MY_THREAD_EXIT(name);
}
                              
/* ------------------------------------------------------- */
/* FUNCTION  funct_4():                                    */
/*   A function to be run as a thread.                     */
/* ------------------------------------------------------- */

void  funct_4(int  name)
{
     int  i;
     
     for(counter4 = 0;counter4 < MAX_COUNT;counter4++) {
          for (i = 1; i <= MAX_COUNT; i++)
               printf("         funct_4() executing\n");
          MY_THREAD_YIELD(name);
     }
     MY_THREAD_EXIT(name);
}

                              
/* ------------------------------------------------------- */
/* FUNCTION  Scheduler():                                  */
/*   The scheduler is first called by the main program to  */
/* setup an entry point.  Then, it is re-entered through   */
/* a longjmp() from the THREAD_YIELD() macro.              */
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
        printf("non_match_up es: %d\n",non_match_up);
        if(contador_t>non_match_up)
            longjmp(SCHEDULER, 1);
        else
        {
            MY_THREAD_JOIN();
        }
     }
}
                         
/* ------------------------------------------------------- */
/*              The main program starts here               */
/*   The main program calls each threads, allowing then to */
/*   initializing themselves.  Then, calls the scheduler.  */
/*   Finally, jumps to the scheduler to start the system.  */
/* ------------------------------------------------------- */

int main(void)
{
     Head = Current = NULL;        /* initialize pointers  */
     
     MY_THREAD_CREATE(funct_1, 1);    /* initialize threads   */
     MY_THREAD_CREATE(funct_2, 2);
     MY_THREAD_CREATE(funct_3, 3);
     MY_THREAD_CREATE(funct_4, 4);
     
     if (setjmp(MAIN) == 0)        /* initialize scheduler */   
         Scheduler();
     
     if(!join)
        longjmp(SCHEDULER,1);         /* start scheduler      */
     
     
     PCB_ptr control = MY_THREAD_getARG(getThread(3));
         
     printf("El hilo que encontro tiene por nombre %d y su estado es %d\n",control->Name,control->Estado);
     printf("termine de ejecutar");
     return 0;
}
