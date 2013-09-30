#include <stdio.h>

#include "mypthread.h"

void *f1(int number);
void *f2(int number);

int main()  {
	contador_t = 0;
	join = 0;
	Head = Current = NULL;        /* initialize pointers  */

	if (setjmp(MAIN) == 0)        /* initialize scheduler */   
        	Scheduler();

	MY_THREAD_CREATE(f1,1);
	MY_THREAD_CREATE(f2,2);

	if(!join)
		longjmp(SCHEDULER,1);         /* start scheduler      */

   	return 0;  
}

void *f1(int number){
  int i;
  i = number;
  //sleep(1);
  printf("f1: %d",i);
  MY_THREAD_EXIT(number); 
}

void *f2(int number){
  int i;
  i = number;
  //sleep(1);
  printf("f2: %d",i);
  MY_THREAD_EXIT(number); 
}
