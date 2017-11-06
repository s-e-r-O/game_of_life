#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* SERVER FUNCTIONS */

int server_init(int portnum, int type);
int wait_for_clients(int server_fd,int server_fd6, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state);
int wait_for_slaves(int server_fd,int server_fd6, int dim, int slaves_state[]);

/* CLIENT FUNCTIONS */

int connect_to_servers(int init_portnum, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state);
int connect_to_master(int init_portnum, int id, int state);

/* GAME OF LIFE CONNECTION FUNCTIONS */

// States for the slaves

#define ALIVE 10
#define DEAD 20
#define NOT_SET 30

// Struct used to send states through sockets connections

struct state_msg{
	int id;	
	int state;
};

void array_state_reset(int n, int array_state[]);