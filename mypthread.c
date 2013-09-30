#include  <stdio.h>
#include  <stdlib.h>
#include  <setjmp.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "mypthread.h"



//
// Threads
//
int pthread_attr_init(pthread_attr_t *attr) {
	if (!attr) 
		return EINVAL;
	contador_t = 0;
	join = 0;
	return 0;
}

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
        //printf("non_match_up es: %d\n",non_match_up);
        if(contador_t>non_match_up)
            longjmp(SCHEDULER, 1);
        else
        {
            MY_THREAD_JOIN();
        }
     }
}
