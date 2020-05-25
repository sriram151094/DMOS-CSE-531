// Sriram Sundararajan - 1217117447
// Venkatramanan Srinivasan - 1217023522

#include"msgs.h"
struct queue *ports[100];

// Client program send the data to the server port (fixed - 0)
// The message contains an integer array and the client port 
// in which it recieves the response from the server.
void client1() {
	int data[10] = {1,2,3,4,5,6,7,8,9,10};
	int i=1; //first port
	while(1){
		send_msg(ports[0], &data[0], ports[i]);
		struct d_type res = recv_msg(ports[i]);
		int size=sizeof(res.msg)/sizeof(res.msg[0]);
		printf("\nClient1 Received data back from server :\n");
		for(int i=0;i<size;i++)
			printf("%d  ",res.msg[i]);
		sleep(1);
	}
}

void client2() {
	int data[10] = {10,9,8,7,6,5,4,3,2,1};
	int i=2;
	while(1){
		send_msg(ports[0], &data[0], ports[i]);
		struct d_type res = recv_msg(ports[i]);
		int size=sizeof(res.msg)/sizeof(res.msg[0]);
		printf("\nClient2 Received data back from server :\n");
		for(int i=0;i<size;i++)
			printf("%d  ",res.msg[i]);
		sleep(1);
	}
}


void client3() {
	int data[10] = {5,5,5,5,5,5,5,5,5,5};
	int i=2;
	while(1){
		send_msg(ports[0], &data[0], ports[i]);
		struct d_type res = recv_msg(ports[i]);
		int size=sizeof(res.msg)/sizeof(res.msg[0]);
		printf("\nClient1 Received data back from server :\n");
		for(int i=0;i<size;i++)
			printf("%d  ",res.msg[i]);
		sleep(1);
	}
}


void client4() {
	int data[10] = {6,7,8,9,10,5,4,3,2,1};
	int i=2;
	while(1){
		send_msg(ports[0], &data[0], ports[i]);
		struct d_type res = recv_msg(ports[i]);
		int size=sizeof(res.msg)/sizeof(res.msg[0]);
		printf("\nClient4 Received data back from server :\n");
		for(int i=0;i<size;i++)
			printf("%d  ",res.msg[i]);
		sleep(1);
	}
}


// Server recives message from a client. Process the message.
// It unpacks the client port from the message payload data and then send the response to the client port.
void server() {
	while(1){
		struct d_type res = recv_msg(ports[0]);
		int msg[10];
		int size=sizeof(res.msg)/sizeof(res.msg[0]);
		printf("\nServer received data from client :\n");
		for(int i=0;i<size;i++)
			printf("%d  ",res.msg[i]);
		for(int i=0;i<size;i++)
			msg[i]=res.msg[i]*2;
		send_msg(res.client_port, &msg[0], ports[0]);
		sleep(1);	
	}
}



void main(){
	
	//creating 100 ports, 0th port is for server, all clients know this port
	for(int i=0;i<100;i++)
		ports[i]=createPort();

	//Starting server thread
	start_thread(server);
	//Starting client threads
	start_thread(client1);
	start_thread(client2);
	start_thread(client3);
	start_thread(client4);

	run();
	
}
