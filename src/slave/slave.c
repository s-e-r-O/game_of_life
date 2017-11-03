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
#include "utils.h"

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

	int neighbours[MAX_NEIGHBOURS];

	int n_neighbours = find_neighbours(id, dim, neighbours);
	int neighbours_state[n_neighbours];
	neighbours_state_init(n_neighbours, neighbours_state);

	int type = find_slave_type(id, dim);

	int server_fd;
	int init_portnum;
	int state = DEAD;

	// SEND STATE TO MASTER!!
	//FOR LOOP THIS 	|
	//					V
	switch(type){
		case ONLY_SERVER:;
			server_fd = server_init(portnum);
			wait_for_clients(server_fd, n_neighbours, n_neighbours, neighbours, neighbours_state, id, DEAD);
			
		break;
		
		case ONLY_CLIENT:;
			init_portnum = portnum - id;
			connect_to_servers(init_portnum, n_neighbours, n_neighbours, neighbours, neighbours_state, id, ALIVE);
		break;
		case SERVER_CLIENT:;
			server_fd = server_init(portnum);
			wait_for_clients(server_fd, (n_neighbours + 1) / 2, n_neighbours, neighbours, neighbours_state, id, DEAD);

			init_portnum = portnum - id;
			connect_to_servers(init_portnum, n_neighbours - (n_neighbours + 1) / 2, n_neighbours, neighbours, neighbours_state, id, DEAD);
		break;
		case CLIENT_SERVER:;
			init_portnum = portnum - id;
			connect_to_servers(init_portnum, (n_neighbours + 1) / 2, n_neighbours, neighbours, neighbours_state, id, ALIVE);

			server_fd = server_init(portnum);
			wait_for_clients(server_fd, n_neighbours - (n_neighbours + 1) / 2, n_neighbours, neighbours, neighbours_state, id, ALIVE);
		break;
	}
	int alive = 0;
	int dead = 0;
	for (int i = 0; i < n_neighbours; i++){
		switch (neighbours_state[i]){
			case ALIVE:
				alive++;
				break;
			case DEAD:
				dead++;
				break;
		}
	}
	if (alive + dead == n_neighbours){
		//printf("%02d: All complete!\n", id);
		if (state == DEAD && alive == 3){
			state = ALIVE;
		} else if (state == ALIVE && !(alive >= 2 && alive <= 3)) {
			state = DEAD;
		}
	}
	//END LOOP THIS		^
	//					|
	exit(EXIT_SUCCESS);
}