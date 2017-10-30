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

	printf ("Proceso esclavo %02d ejecutándose en una matriz %dx%d.\n", id, dim, dim);
	printf ("Proceso esclavo, esuchando en el puerto %d.\n", portnum);

	exit(EXIT_SUCCESS);
}