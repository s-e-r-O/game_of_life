#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "master_utils.h"

/* Used only to get those states */
#include "connection.h"

void usage(char *name) {
	printf ("Usage: %s -d dim -p portnumber\n", name);
	printf ("       Argument is mandatory.\n");
	printf ("        * dim indicates the matrix dimension.\n");
	printf ("        * portnumber must be greater than 1024.\n");
	exit(EXIT_FAILURE);
}

void get_args(int argc, char **argv, int *dim, int *portnum){

	if (argc != 5)
		usage(argv[0]);

	opterr = 0;
	int c;
	char *str = NULL;


	while((c = getopt(argc, argv, "d:p:")) != -1)
		switch (c)
		{
			case 'd':
				str = optarg;
				*dim = strtoul(str, NULL, 10);
				break;
			case 'p':
				str = optarg;
				*portnum = strtoul(str, NULL, 10);
				break;
			default:
				usage(argv[0]);
				break;
	if (str == NULL || *portnum < 1024)
		usage(argv[0]);
	}
}

void master_shutdown(){

	endwin();
	exit(EXIT_SUCCESS);
}

void show_generation(WINDOW *win, int gen){
	wclear(win);

	wprintw(win, "Generation: %d", gen);

	wrefresh(win);
}

void show_slaves(WINDOW *win, int dim, int states[]){

	wclear(win);

	box(win, 0, 0);
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			switch(states[i * dim + j]){
				case ALIVE:
					mvwaddch(win, i + 1, j + 1, ACS_CKBOARD);
				break;
				case DEAD:
					mvwaddch(win, i + 1, j + 1, ' ');
				break;
				case NOT_SET:
					mvwaddch(win, i + 1, j + 1, ACS_BULLET);
				break;
				}
			}		
		}

	wrefresh(win);
}
