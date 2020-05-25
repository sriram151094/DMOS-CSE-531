/*
Sriram Sundararajan -- 1217117447
Venkatramanan Srinivasan -- 1217023522
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sem.h"

semaphore_t *rSem, *wSem, *mutex; 
int rwc = 0, wwc = 0, rc = 0, wc = 0, gId=0;

void readerEnter(int id)
{
	printf("Reader: %d is trying to read\n", id);
	P(mutex);
	if (wwc > 0 || wc > 0) 
	{
	    printf("Reader: %d is blocking for writer\n", id);
	    rwc++;		
	    V(mutex);		
	    P(rSem);
	    rwc--;
	}
	rc++;
	if (rwc > 0) V(rSem);
	else V(mutex);
}

void readerExit(int id)
{
	P(mutex);
	rc--;
	if (rc == 0 && wwc > 0) 
	{
	    V(wSem);
	} else V(mutex);

}

void writerEnter(int id)
{
	printf("Writer: %d\ttrying to write\n", id);
	P(mutex);
	if (rc > 0 || wc > 0)
	{
	    printf("Writer: %d is blocking others\n", id);
	    wwc++;
	    V(mutex);
	    P(wSem);
	    wwc--;
	}
	wc++;
	V(mutex);
}

void writerExit(int id)
{
	P(mutex);
	wc--;
	if (rwc > 0) V(rSem); 
	else if (wwc > 0) V(wSem);
	else V(mutex);
}

void reader(void)
{ 
  int id;
  P(mutex); id = gId++; V(mutex);
  while(1)
  {
	readerEnter(id);
	printf("\n\nReader %d is Reading\n\n", id);
	sleep(1);
	readerExit(id);
  }
}

void writer(void)
{
  int id;
  P(mutex); id = gId++; V(mutex);
  while(1)
  {
 	writerEnter(id);
	printf
	    ("\n\nWriter: %d is Writing\n\n", id);
	sleep(1);
	writerExit(id);
  }
}

int main()
{
    mutex = create_sem(1);
    rSem = create_sem(0);
    wSem = create_sem(0);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);
	start_thread(reader);
    start_thread(reader);
	start_thread(reader);
    start_thread(reader);
    start_thread(writer);
    start_thread(writer);
    run();
}   





