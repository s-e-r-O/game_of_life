#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "connection.h"
#include "slave_utils.h"

int main(int argc, char **argv)
{
	int id = 0; 			/* identifies the slave in the matrix 	*/
	int dim = 0;		 	/* indicates the matrix dimension		*/
	int portnum = 0;		/* port used as a server 				*/

	get_args(argc, argv, &id, &dim, &portnum);

	int neighbours[MAX_NEIGHBOURS]; /* Stores the id of all neighbours */

	int n_neighbours = find_neighbours(id, dim, neighbours);
	int neighbours_state[n_neighbours]; /* Stores the state of all neighbours */

	int server_fd;

	/* 
		Tyṕe can be one of four:

			* SERVER_ONLY 	: 	Slaves that start as a server and don't switch to client in any moment.
								(Even column on an even row in the matrix)
			* CLIENT_ONLY 	:	Slaves that start as a client and don't switch to server in any moment.
								(Odd column on an even row in the matrix)
			* SERVER_CLIENT	:	Slaves that start as a server and switch to client when needed.
								(Odd column on an odd row in the matrix)
			* CLIENT_SERVER	:	Slaves that start as a client and switch to server when needed.
								(Even column on an odd row in the matrix)
	*/
	int type = find_slave_type(id, dim); 

	/* server_fd won't be needed at all on an ONLY_CLIENT slave */
	if (type != ONLY_CLIENT){
		server_fd = server_init(portnum);
	}

	/* Initial port number */
	int init_portnum = portnum - id;
	
	/* 
		State can be one of three:

			* ALIVE
			* DEAD
			* NOT_SET	(Used for checking if a connection was already made between two slaves)
	*/
	int state;
	
	/************************************************

		Simplest Oscillator

									O
			OOO			<->			O 	
									O
	
	************************************************/
	int center_point = dim*((dim+1)/2)-(dim/2);

	if (id >= center_point-1 && id <= center_point+1 ){
		printf("%02d: I'm ALIVE\n", id);
		state = ALIVE;
	}
	else {
		state = DEAD;
	}

	/* Let's give some time to the master, shall we? */
	sleep(2);
	
	while(1){

		array_state_reset(n_neighbours, neighbours_state);
		
		connect_to_master(init_portnum, id, state);
		
		switch(type){
			case ONLY_SERVER:;
				
				/* Waiting for all neighbours */
				wait_for_clients(server_fd, n_neighbours, n_neighbours, neighbours, neighbours_state, id,state);
				
			break;
			
			case ONLY_CLIENT:;
				
				/* Connecting to all neighbours */
				connect_to_servers(init_portnum, n_neighbours, n_neighbours, neighbours, neighbours_state, id, state);
			
			break;
			case SERVER_CLIENT:;

				/* 
					Waiting for all client neighbours '(neighbours + 1)/2'.

					CORNER: Neighbours = 3
							Client Neighbours = 2

					SIDE:	Neighbours = 5
							Client Neighbours = 3

					MIDDLE:	Neighbours = 8
							Client Neighbours = 4 (Keep in mind that the '/' division of int numbers is rounded to the lowest int number)

				*/

				int total_clients = (n_neighbours + 1) / 2;
				wait_for_clients(server_fd, total_clients, n_neighbours, neighbours, neighbours_state, id,state);

				/* After switching from server to client, connecting to the remaining servers */
				connect_to_servers(init_portnum, n_neighbours - total_clients, n_neighbours, neighbours, neighbours_state, id,state);
			
			break;
			case CLIENT_SERVER:;
				
				/* 
					Connecting to all server neighbours '(neighbours + 1)/2'.

					CORNER: Neighbours = 3
							Server Neighbours = 2

					SIDE:	Neighbours = 5
							Server Neighbours = 3

					MIDDLE:	Neighbours = 8
							Server Neighbours = 4 (Keep in mind that the '/' division of int numbers is rounded to the lowest int number)

				*/
				int total_servers = (n_neighbours + 1) / 2;
				connect_to_servers(init_portnum, total_servers, n_neighbours, neighbours, neighbours_state, id, state);

				/* After switching from client to server, waiting for the remaining clients */
				wait_for_clients(server_fd, n_neighbours - total_servers, n_neighbours, neighbours, neighbours_state, id, state);

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

		// Checking that all neighbours have a set state
		if (alive + dead == n_neighbours){
			
			printf("%02d: All complete!\n", id);
			
			if (state == DEAD && alive == 3){
				state = ALIVE;
			} else if (state == ALIVE && !(alive >= 2 && alive <= 3)) {
				state = DEAD;
			}
		}

	}
	
	exit(EXIT_SUCCESS);
}