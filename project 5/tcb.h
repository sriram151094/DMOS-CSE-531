#include<ucontext.h>
#include<string.h>
struct TCB_t
{
	ucontext_t context;
	int thread_id;
};

struct TCB_t_server
{
	ucontext_t context;
	int thread_id;
	char data[1024];
};
void init_TCB (struct TCB_t *tcb, void *function, void *stackP, int stack_size)
{
    memset(tcb, '\0', sizeof(tcb));    
    getcontext(&tcb->context);              
    tcb->context.uc_stack.ss_sp = stackP;
    tcb->context.uc_stack.ss_size = (size_t) stack_size;
    makecontext(&tcb->context, function,1, tcb->thread_id);
}


void init_TCB_server (struct TCB_t_server *tcb, void *function, void *stackP, int stack_size)
{
    memset(tcb, '\0', sizeof(tcb));    
    getcontext(&tcb->context);              
    tcb->context.uc_stack.ss_sp = stackP;
    tcb->context.uc_stack.ss_size = (size_t) stack_size;
    makecontext(&tcb->context, function,2,tcb->data, tcb->thread_id);
}