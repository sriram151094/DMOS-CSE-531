// Sriram Sundararajan - 1217117447
// Venkatramanan Srinivasan - 1217023522

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include"msgs.h"
struct queue *ports[100];

// Client program send the data to the server port (fixed - 0)
// The message contains an integer array and the client port 
// in which it recieves the response from the server.

int num_clients = 0;
int client_port = 1;
static int local_server_count=0;
static int local_servers=0;
semaphore_t *mutex;

char **files;

void client() {
	int port;
	P(&mutex);
		port = client_port++;	
	V(&mutex);

	int send_file = 0;
	char send_str[32];
	while(1) {
		printf("Client %i sending filename\n",port);
		struct q_node* init_msg = createItem(files[port-1], ports[port], 1, port);
		send_msg(ports[0], init_msg);
		struct d_type response = recv_msg(ports[port]);
		if (response.actionType == 0) {
			/* Server is ready for transfer */
			send_file = 1;
			break;
		}
		else if (response.actionType == -1) {
			/* Filename length is not acceptable */
			break; 
		}
		sleep(1);
	}

	if (send_file) {
	    
		struct d_type ack;
		int done_flag = 1;
		//printf("Sending file from client \n");
		FILE *fp = fopen(files[port-1], "r");
		while(fgets(send_str, 32, fp) != NULL) {
			send_msg(ports[0], createItem(send_str, ports[port], 2, port));
			ack = recv_msg(ports[port]);
			if(ack.actionType == -1){
				/* Max file size reached */
				printf("Transfer stopped for %s", files[port-1]);
				break;
			}
		}
	 
		send_msg(ports[0], createItem("", ports[port], 3, port));
	}

	while(1) { yield(); }
}


// Server recives message from a client. Process the message.
// It unpacks the client port from the message payload data and then send the response to the client port.
void server() {
	int num_requests = 0;
	char server_files[100][32];

	while(1){
		struct d_type message = recv_msg(ports[0]);
		//printf("inside Server - action type%d\n", message.actionType);
		int reply_port = message.port_num;

		if(message.actionType == 1) 
		{
			if (num_requests > 3) {
				struct q_node* error_msg= createItem("Server is busy!", ports[0], -2, 0);
				send_msg(ports[reply_port], error_msg);
			}
			else {
				++num_requests;
				char * fileName = (char*) malloc(strlen(message.msg_data));
				strcpy(fileName, message.msg_data);
				if(strlen(fileName) > 15) {
					struct q_node* error_msg= createItem("File name exceeds 15 characters", ports[0], -1, 0);
					send_msg(ports[reply_port], error_msg);
				}
				else {
					strncpy(server_files[reply_port], fileName, 15);
					strcat(server_files[reply_port], ".server");
					struct q_node* succ_msg = createItem("Filename accepted!", ports[0], 0, 0);
					send_msg(ports[reply_port], succ_msg);
				}
			}
		}
		else if (message.actionType == 2) {
			struct stat st;
			stat(server_files[reply_port], &st);
			if (st.st_size >= (1024*1024)) {
				send_msg(ports[reply_port], createItem("File size is more than 1MB", ports[reply_port], -1, 0));
			}
			else {
				FILE *fp = fopen(server_files[reply_port], "a");
				char buffer[32];
				memcpy(buffer, message.msg_data, 32);
				printf("Received %s from %i\n", buffer, reply_port);
				fprintf(fp, "%s", buffer);
				send_msg(ports[reply_port], createItem("Write is successful!", ports[reply_port], 0, 0));
				fclose(fp);
			}
		}
		else if (message.actionType == 3) {
			/* Marks end of file transfer for a client */
			--num_requests;
		}
	}
}

int main(int argc,char *argv[] ){	

	mutex = create_sem(1);
	//creating 100 ports, 0th port is for server, all clients know this port
	for(int i=0;i<100;i++){
		ports[i]=createPort();
		printf("Port  %p\n", ports[i]);
	}

	//Starting server thread
	start_thread(server);
	//Starting client thread
	num_clients = strtoul(argv[1], 0, 10);
	files = malloc(num_clients * sizeof (*files));
	for (int i = 0; i < num_clients; ++i) {
		int filename_len = strlen(argv[i+2]);
		files[i] = malloc((filename_len+1) * sizeof (*files[i]));
		memset(files[i], 0, filename_len+1);
		memcpy(files[i], argv[i+2], filename_len);
		printf("filename: %s\n", files[i]);
		start_thread(client);
	}
	run();
	return 0;
}
