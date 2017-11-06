#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>

#include "connection.h"
#include "master_utils.h"

int main(int argc, char **argv)
{
	signal(SIGINT, master_shutdown);

	int portnum = 0;
	int dim = 0;


	get_args(argc, argv, &dim, &portnum);

	int slaves_state[dim*dim];
	

	
	int generation = 0;

	int server_fd = server_init(portnum, 4);
	int server_fd6 = server_init(portnum, 6);

	sleep(1);
	
	setlocale(LC_ALL, "");

	initscr();
	WINDOW *board = newwin(dim + 2, dim + 2, (LINES- dim - 2) / 2, (COLS - dim - 2)/2);
	WINDOW *info = newwin(1, COLS, 0, 0);

	while (1){
		
		array_state_reset(dim*dim, slaves_state);
		
		wait_for_slaves(server_fd, server_fd6, dim, slaves_state);
		
		show_slaves(board, dim, slaves_state);
		show_generation(info, generation);

		generation++;
	}

}
