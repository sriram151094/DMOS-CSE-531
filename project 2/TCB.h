#include<ucontext.h>
#include<string.h>
struct TCB_t
{
	ucontext_t context;
	int thread_id;
};

void init_TCB (struct TCB_t *tcb, void *function, void *stackP, int stack_size)
{
    memset(tcb, '\0', sizeof(tcb));
    getcontext(&tcb->context);
    tcb->context.uc_stack.ss_sp = stackP;
    tcb->context.uc_stack.ss_size = (size_t) stack_size;
    makecontext(&tcb->context, function, 1, tcb->thread_id);
}
