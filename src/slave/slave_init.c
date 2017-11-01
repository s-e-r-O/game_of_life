#include "slave_init.h"
#include <stdio.h>
int max (int a, int b){
	if (a>b)
		return a;
	return b;
}

int min(int a, int b){
	if (a < b)
		return a;
	return b;
}
int find_neighbour_ports(int id, int dim, int portnum, int neighbours_port[]){
	int row, col;
	row = (id - 1) / dim + 1;
	col = (id - 1) % dim + 1;
	int n_neighbours = 0;
	int i,j;

	for (i = max(row - 1, 1); i<=min(row+1, dim); i++){
		for (j = max(col - 1, 1); j<=min(col +1, dim); j++){
			if (i != row || j != col){
				neighbours_port[n_neighbours++] = (portnum - id) + (i - 1) * dim + j;
			}
		}
	}

	return n_neighbours;
/*
	if (row < dim){
		neighbours_port[n_neighbours++] = portnum + dim
	}
	if (col > 1)
	
	if (id == 1){
		//printf("ESQ_NW\n");
		neighbours_port[0] = portnum + 1;
		neighbours_port[1] = portnum + dim;
		neighbours_port[2] = portnum + dim + 1;
		return 3;
	}
	if (id == dim){
		//printf("ESQ_NE\n");
		neighbours_port[0] = portnum - 1;
		neighbours_port[1] = portnum + dim;
		neighbours_port[2] = portnum + dim - 1;
		return 3;
	}
	if (id == dim*dim){
		//printf("ESQ_SE\n");
		neighbours_port[0] = portnum - 1;
		neighbours_port[1] = portnum - dim;
		neighbours_port[2] = portnum - dim - 1;
		return 3;
	}
	if (id == dim*(dim-1) + 1){
		//printf("ESQ_SW\n");
		neighbours_port[0] = portnum + 1;
		neighbours_port[1] = portnum - dim;
		neighbours_port[2] = portnum - dim + 1;
		return 3;
	}
	if (id < dim){
		//printf("LADO_N\n");
		neighbours_port[0] = portnum + 1;
		neighbours_port[1] = portnum + dim + 1;
		neighbours_port[2] = portnum + dim - 1;
		neighbours_port[3] = portnum + dim;
		neighbours_port[4] = portnum - 1;
		return 5;
	}
	if (id > dim*(dim-1)){
		//printf("LADO_S\n");
		neighbours_port[0] = portnum + 1;
		neighbours_port[1] = portnum - 1;
		neighbours_port[2] = portnum - dim;
		neighbours_port[3] = portnum - dim + 1;
		neighbours_port[4] = portnum - dim - 1;
		return 5;
	}
	if ((id - 1) % dim == 0){
		//printf("LADO_W\n");
		neighbours_port[0] = portnum + 1;
		neighbours_port[1] = portnum + dim;
		neighbours_port[2] = portnum - dim;
		neighbours_port[3] = portnum + dim + 1;
		neighbours_port[4] = portnum - dim + 1;
		return 5;
	}
	if (id%dim == 0){
		//printf("LADO_E\n");
		neighbours_port[0] = portnum - 1;
		neighbours_port[1] = portnum + dim;
		neighbours_port[2] = portnum - dim;
		neighbours_port[3] = portnum + dim - 1;
		neighbours_port[4] = portnum - dim - 1;
		return 5;
	}
	//printf("MEDIO\n");
	neighbours_port[0] = portnum - 1;
	neighbours_port[1] = portnum + 1;
	neighbours_port[2] = portnum - dim;
	neighbours_port[3] = portnum + dim;
	neighbours_port[4] = portnum - dim - 1;
	neighbours_port[5] = portnum - dim + 1;
	neighbours_port[6] = portnum + dim - 1;
	neighbours_port[7] = portnum + dim + 1;
	return 8;
	*/
}

int find_initial_type(int id, int dim){
	if (dim % 2 != 0){
		if (id % 2 != 0){
			return SERVER;
		} else {
			return CLIENT;
		}
	} else {
		if ((id % 2 != 0 && ((id - 1) / dim) % 2 == 0) || (id % 2 == 0 && ((id - 1) / dim) % 2 != 0 )){
			return SERVER;
		} else {
			return CLIENT;
		}
	}

}
