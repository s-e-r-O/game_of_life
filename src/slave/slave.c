#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "server.h"
#include "client.h"
#include "slave_init.h"

#define SERVER 0
#define CLIENT 1

void
usage(char *name)
{
	printf ("Usage: %s -i id -d dim -p portnumber\n", name);
	printf ("       Arguments are mandatory.\n");
	printf ("        * id identifies the slave in the matrix.\n");
	printf ("        * dim indicates the matrix dimension.\n");
	printf ("        * portnumber must be greater than 1024.\n");
	exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
	int id = 0;
	int dim = 0;
	int portnum = 0;

	// Procesar argumentos de la línea de comando

	if (argc != 7)
		usage(argv[0]);

	
	opterr = 0;
	int c;
	char *str = NULL;

	while((c = getopt(argc, argv, "i:d:p:")) != -1)
		switch (c)
		{
			case 'i':
				str = optarg;
				id = strtoul(str, NULL, 10);
				break;
			case 'd':
				str = optarg;
				dim = strtoul(str, NULL, 10);
				break;
			case 'p':
				str = optarg;
				portnum = strtoul(str, NULL, 10);
				break;
			default:
				usage(argv[0]);
				break;
		}

	if (str == NULL || portnum < 1024)
 		usage(argv[0]);

 	// Initialization of slave

	int neighbours_port[MAX_NEIGHBOURS];	
	int n_neighbours = find_neighbour_ports(id, dim, portnum, neighbours_port);
	
	int initial_type = find_initial_type(id, dim);

	int server_fd;

	int i;

	switch(initial_type){
		case SERVER:
			//printf("Proceso esclavo %02d ejecutandose en una matriz %dx%d, escuchando el puerto %d como Server\n", id, dim, dim, portnum);
			// -------- INITIALIZATION
			
			server_fd = server(portnum);
			
			// -------------
			// ----------- LISTEN
			int new_socket;
    		struct sockaddr_in address;
    		int addrlen = sizeof(address);

    		int n_clients;
    		switch(n_neighbours){
    			case 3:
    				n_clients = 2;
    			break;
    			case 5:
    				n_clients = 3;
    			break;
    			case 8:
    				n_clients = 4;
    			break;
    		}

			printf("%02d: Listening...\n", id);
			
			listen(server_fd, 3);
			// ---------
			for (i = 0; i < n_clients; i++){
				new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
				int peer_id;
				read(new_socket, &peer_id, 4);
				printf("%02d: Connected to %02d (Port: %d)...\n", id, peer_id, address.sin_port);
			}
			
			if ((id - 1) / dim % 2 == 0) { // even row (counting from 0)
				printf("%02d: Switching to client\n", id);
			} else {
				for (i = n_clients; i < n_neighbours; i++){
					new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
					int peer_id;
					read(new_socket, &peer_id, 4);
					printf("%02d: Connected to %02d (Port: %d)...\n", id, peer_id, address.sin_port);
				}	
			}
		break;
		case CLIENT:
			sleep (1); //waiting for all servers to be initialized
		
			int n_servers;
		
    		switch(n_neighbours){
    			case 5:
    				n_servers = 3;
    			break;
    			case 8:
    				n_servers = 4;
    			break;
    		}
				
			for (i = 0; i < n_neighbours; i++){
				printf("%02d: Trying to connect to %02d...", id, neighbours_port[i] - (portnum - id));
				//int n_failed = 0;
				int sock = client(neighbours_port[i]);
				//while ((sock = client(neighbours_port[i])) < 0 || n_failed > 1){
				//	n_failed ++;
				//}
				if (sock < 0){
					printf("failed\n");
					continue;
				}
				send(sock, &id, sizeof(id), 0);
				printf("Success!\n");	
			}

			if ((id - 1) / dim % 2 == 0) { // even row (counting from 0)
				printf("%02d: Switching to server\n", id);
			} else {
				/*sleep(1);
				for (i = n_servers; i < n_neighbours; i++){
					printf("%02d: Trying to connect to %02d...", id, neighbours_port[i] - (portnum - id));
					//int n_failed = 0;
					int sock = client(neighbours_port[i]);
					//while ((sock = client(neighbours_port[i])) < 0 || n_failed > 1){
					//	n_failed ++;
					//}
					if (sock < 0){
						printf("failed\n");
						continue;
					}
					send(sock, &id, sizeof(id), 0);
					printf("Success!\n");	
				}*/	
			}
			//printf("Proceso esclavo %02d ejecutandose en una matriz %dx%d, escuchando el puerto %d como Client\n", id, dim, dim, portnum);
			//client(n_neighbours, neighbours_port);
		break;
	}

	exit(EXIT_SUCCESS);
}