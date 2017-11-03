#include "slave_init.h"
#include <stdio.h>

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
