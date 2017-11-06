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

	int server_fd = server_init(portnum);

	sleep(1);
	
	setlocale(LC_ALL, "");

	initscr();
	WINDOW *board = newwin(dim + 2, dim + 2, (LINES- dim - 2) / 2, (COLS - dim - 2)/2);
	WINDOW *info = newwin(1, COLS, 0, 0);

	while (1){
		wclear(board);
		wclear(info);

		wprintw(info, "Generation: %d", generation);
		array_state_reset(dim*dim, slaves_state);
		
		wait_for_slaves(server_fd, dim, slaves_state);
		

    	box(board, 0, 0);
		
		for (int i = 0; i < dim; i++){
			for (int j = 0; j < dim; j++){
				switch(slaves_state[i * dim + j]){
					case ALIVE:
						mvwaddch(board, i + 1, j + 1, ACS_CKBOARD);
					break;
					case DEAD:
						mvwaddch(board, i + 1, j + 1, ' ');
					break;
					case NOT_SET:
						mvwaddch(board, i + 1, j + 1, ACS_BULLET);
						break;
				}
			}
			
		}
		wrefresh(board);
		wrefresh(info);
		
		generation++;

		//printf("M: New generation %d!\n", generation);
	}

}
