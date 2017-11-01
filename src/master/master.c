#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void
usage(char *name)
{
	printf ("Usage: %s -p portnumber\n", name);
	printf ("       Argument is mandatory.\n");
	printf ("        * portnumber must be greater than 1024.\n");
	exit(EXIT_FAILURE);
}

void createPorts();

int
main(int argc, char **argv)
{
	int portnum = 0;

	// Procesar argumentos de la línea de comando

	if (argc != 3)
		usage(argv[0]);

	opterr = 0;
	int c;
	char *port = NULL;

	while((c = getopt(argc, argv, "p:")) != -1)
		switch (c)
		{
			case 'p':
				port = optarg;
				portnum = strtoul(port, NULL, 10);
				break;
			default:
				usage(argv[0]);
				break;
		}

	if (port == NULL || portnum < 1024)
		usage(argv[0]);

	printf ("Proceso maestro ejecutándose.\n");
	printf ("Proceso maestro, esuchando en el puerto %d.\n", portnum);

	exit(EXIT_SUCCESS);
}