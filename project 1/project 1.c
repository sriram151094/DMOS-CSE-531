/*
Team members:
Sriram Sundararajan  --- 1217117447
Venkatramanan Srinivasan --- 1217023522
*/

#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

semaphore_t mutex,print,t[3]; 

void thread_1(int *array)
{
	while(1)
	{
		P(t);
		P(&mutex);
		array[0]++;
		printf("Child 1 incremented value of array element 1 by 1\n");
		sleep(1);
		V(&mutex);
		V(&print);
	}
}

void thread_2(int *array)
{
	while(1)
	{
		P(t+1);
		P(&mutex);
		array[1]++;
		printf("Child 2 incremented value of array element 2 by 1\n");
		sleep(1);
		V(&mutex);
		V(&print);
	}
}

void thread_3(int *array)
{
	while(1)
	{
		P(t+2);
		P(&mutex);
		array[2]++;
		printf("Child 3 incremented value of array element 3 by 1\n");
		sleep(1);
		V(&mutex);
		V(&print);
	}
}


int main()
{

	int array[3] = {0,0,0};
	init_sem(&mutex, 1);
	init_sem(&print, 0);
	init_sem(t, 1);
	init_sem(t+1, 1);
	init_sem(t+2, 1);
    start_thread(thread_1, array); 
    start_thread(thread_2, array);
    start_thread(thread_3, array);

    while(1)
    {	
    	
    	P(&print);
    	P(&print);
    	P(&print);	//Will wait for the threads to complete updating the variables before printing.
    	P(&mutex);
    	printf("Printing from main function:\n");
    	for (int i = 0; i < 3; i++)
    	{
    		printf("Array element %d: %d\n", i+1, array[i]);
    	}
    	V(&mutex);
    	V(t);
    	V(t+1);
    	V(t+2);
    	sleep(1);
    	
    }
}