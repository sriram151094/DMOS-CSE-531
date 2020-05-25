#include<stdio.h>
#include"q.h"

struct node* ReadyQ;
struct node* Curr_Thread;
int counter_thread=0;
void start_thread(void (*function)(int))
{ 
  void *stack = malloc(8192);
  struct TCB_t *tcb= (struct TCB_t *) malloc(sizeof(struct TCB_t));
  tcb->thread_id = ++counter_thread;
  init_TCB(tcb,function,stack,8192);
  struct node* newItem1 = newItem();
  newItem1->tcb_t = tcb;
  AddQueue(&ReadyQ,newItem1);
}

void run()
{  
    Curr_Thread = DelQueue(&ReadyQ);
    ucontext_t parent;
    getcontext(&parent);
    swapcontext(&parent, &(Curr_Thread ->tcb_t->context));
}

void yield()
{
  struct node *Prev_Thread;
  AddQueue(&ReadyQ, Curr_Thread);
  Prev_Thread = Curr_Thread;
  Curr_Thread = DelQueue(&ReadyQ);
  swapcontext(&(Prev_Thread ->tcb_t->context),&(Curr_Thread ->tcb_t->context));
}
