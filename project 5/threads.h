#include<stdio.h>
#include"q.h"

struct node* ReadyQ = 0;
struct node* Curr_Thread = 0;
int counter_thread_server=0;
int counter_thread=0;
void start_thread(void (*function)(void))
{  
  printf("Initializing thread\n");
  void *stack = malloc(8192);
  struct TCB_t *tcb= (struct TCB_t *) malloc(sizeof(struct TCB_t)); 
  tcb->thread_id = ++counter_thread;
  init_TCB(tcb,function,stack,8192);
  struct node* newItem1 = newItem();
  newItem1->tcb_t = tcb;
  AddQueue(&ReadyQ,newItem1);
}

void start_server_thread(void (*function)(char* fileName), char* fileName, int port_no)
{  
  void *stack = malloc(8192);
  struct TCB_t_server *tcb= (struct TCB_t_server *) malloc(sizeof(struct TCB_t_server)); 
  tcb->thread_id = ++counter_thread;
  //tcb->data = (char *) malloc(strlen(fileName));
  strcpy(tcb->data , fileName);
  init_TCB_server(tcb,function,stack,8192);
  struct node* newItem1 = newItem();
  newItem1->tcb_t = tcb;
  AddQueue(&ReadyQ,newItem1);
}

void run()
{   
  ucontext_t parent;     
  getcontext(&parent);  
  swapcontext(&parent, &(ReadyQ->tcb_t->context));
}

 
void yield() 
{
  struct node *Prev_Thread = ReadyQ;
  AddQueue(&ReadyQ, deleteItem(&ReadyQ));
  swapcontext(&(Prev_Thread ->tcb_t->context),&(ReadyQ ->tcb_t->context));
}
