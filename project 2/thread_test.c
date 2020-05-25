/*
Sriram Sundararajan -- 1217117447
Venkatramanan Srinivasan -- 1217023522

*/
#include"threads.h"
#include<stdio.h>
int g=0;

void function1(int thread_id)
{
    int local_var = 1;
    while(1)
    {
        g++;
	printf("Thread ID - %d\n", thread_id);
	printf("Printing the value of local variable from Function 1:     %d\n",local_var++);
        printf("Printing the value of global variable from Function 1:    %d\n",g);
        sleep(1);
        yield();
    }
}

void function2(int thread_id)
{	
    int local_var = 1;
    while(1)
    {
        g++;
	printf("Thread ID - %d\n", thread_id);
	printf("Printing the value of local variable from Function 2:     %d\n",local_var++);
        printf("Printing the value of global variable from Function 2:    %d\n",g);
        sleep(1);
        yield();
    }
}

void function3(int thread_id)
{
    int local_var = 1;
    while(1)
    {
    	printf("Thread ID - %d\n", thread_id);
	g++;
	printf("Printing the value of local variable from Function 3:     %d\n",local_var++);
        printf("Printing the value of global variable from Function 3:    %d\n",g);
        sleep(1);
        yield();
    }
}

int main()
{
    g=0;
    start_thread(function1);
    start_thread(function2);
    start_thread(function3);
    run();
    return 0;
}
