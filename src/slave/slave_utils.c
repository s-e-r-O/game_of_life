#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "slave_utils.h"

void usage(char *name) {
	printf("Usage: %s -i id -d dim -p portnumber\n", name);
	printf("       Arguments are mandatory.\n");
	printf("        * id identifies the slave in the matrix.\n");
	printf("        * dim indicates the matrix dimension.\n");
	printf("        * portnumber must be greater than 1024.\n");
	exit(EXIT_FAILURE);
}

void get_args(int argc, char **argv, int *id, int *dim, int *portnum){
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
				*id = strtoul(str, NULL, 10);
				break;
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
		}

	if (str == NULL || *portnum < 1024)
 		usage(argv[0]);
}

int find_neighbours(int id, int dim, int neighbours[]){

	int row, col;
	row = (id - 1) / dim + 1;
	col = (id - 1) % dim + 1;
	int n_neighbours = 0;
	
	int i,j;

	for (i = row - 1; i <= row + 1; i++){
		for (j = col - 1; j<=col + 1; j++){

			if (i > 0 && i <= dim && j > 0 && j <= dim && (i != row || j != col)){
				neighbours[n_neighbours++] = (i - 1) * dim + j;
			}
		}
	}

	return n_neighbours;
}

int find_slave_type(int id, int dim){
	int row = (id - 1) / dim + 1;
	int col = (id - 1) % dim + 1;
	if (row % 2 != 0){
		if (col % 2 != 0){
			return SERVER_CLIENT;
		} else {
			return CLIENT_SERVER;
		}
	} else {
		if (col % 2 != 0){
			return ONLY_CLIENT;
		} else {
			return ONLY_SERVER;
		}
	}
}
