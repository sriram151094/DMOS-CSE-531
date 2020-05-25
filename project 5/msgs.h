#include"sem.h"


struct d_type {
  int actionType; //Describes the action intended through the message
	char* msg_data;
	struct queue *client_port;
  int port_num;
};

struct q_node {
 struct d_type data;
 struct q_node *next;
};

struct queue {
 semaphore_t *producer, *consumer, *mutex;	
 int count;
 struct q_node *head;
};

struct queue* createPort() {

  struct queue* newqueue = (struct queue *) malloc(sizeof(struct queue));
  newqueue->count = 0;
  newqueue->head = NULL;
  newqueue-> producer=create_sem(10);  //Maximum of 10 msgs per queue or port
  newqueue-> mutex = create_sem(1);    // One mutex per port 
  newqueue-> consumer = create_sem(0);
  return newqueue;
} 

//Given a port queue add the q_node item which contains the message to the queue.
// Gets called when sending message to the server
void addItemToPortQueue(struct queue* port,struct q_node* item)  
    {  	
       if(port == NULL){
       	printf("NULL port seen.\n");
          return;
       }
       
       P(port->producer);
       P(port->mutex);
       struct q_node *qnode = port->head;
       if(qnode == NULL) {
       	port -> head = item;
       } 
       else {
       		while(qnode->next != NULL)
       			qnode = qnode->next;
       		qnode->next = item;
       }
       V(port->mutex);
       V(port->consumer);
    } 

// Called by the Server to process a message from a client. 
struct d_type deleteQueue(struct queue *port) {
	if(port == NULL){
		printf("NULL port seen.\n");
		return;
	}
	    P(port->consumer);
    	P(port->mutex);

	struct d_type result = port->head->data;
	port->head = port->head->next;
	
	V(port->mutex);
  V(port->producer);
	return result;

}

struct q_node * createItem(char *msg, struct queue *client_port, int actionType, int port_num) {
	struct q_node * new_node = (struct q_node *) malloc(sizeof(struct q_node));
  new_node->data.msg_data = (char*) malloc(strlen(msg));
	strcpy(new_node->data.msg_data, msg);
  new_node->data.actionType = actionType;
	new_node->data.client_port = client_port;
  new_node->data.port_num = port_num;
	new_node->next = NULL;
	return new_node;
}

// Send message to the server - Server port, message and the client port number to get the response are to be passed.
void send_msg(struct queue * port, struct q_node * msg) {
	return addItemToPortQueue(port, msg);
}



// Recieve message from the given port
struct d_type recv_msg(struct queue *port){
	return deleteQueue(port);
}
