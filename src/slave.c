#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
find_pos(int id, int dim, int portnum, int neighbours_port[]){
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

	//printf ("Proceso esclavo %02d ejecutándose en una matriz %dx%d.\n", id, dim, dim);
	//printf ("Proceso esclavo, esuchando en el puerto %d.\n", portnum);
	
	int neighbours_port[8];
	int n_neighbours = find_pos(id, dim, portnum, neighbours_port);
	/*for (int i=0; i < n_neighbours; i++){
		printf("neighbours_port[%d] = %d\n", i, neighbours_port[i]);
	}*/

	if (dim % 2 != 0){
		if (id % 2 != 0){
			printf("Server\n");
		} else {
			printf("Client\n");
		}
	} else {
		if ((id % 2 != 0 && ((id - 1) / dim) % 2 == 0) || (id % 2 == 0 && ((id - 1) / dim) % 2 != 0 )){
			printf("Server\n");
		} else {
			printf("Client\n");
		}
	}

	while(1){

	};

	exit(EXIT_SUCCESS);
}