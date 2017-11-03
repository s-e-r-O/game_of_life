
#define MAX_NEIGHBOURS 8

#define ONLY_SERVER 0
#define ONLY_CLIENT 1
#define SERVER_CLIENT 2
#define CLIENT_SERVER 3

void usage(char *name);
void get_args(int argc, char **argv, int *id, int *dim, int *portnum);

int find_neighbours(int id, int dim, int neighbours[]);
int find_slave_type(int id, int dim);