
#define MAX_NEIGHBOURS 8

#define SERVER 0
#define CLIENT 1
int max(int a, int b);
int min(int a, int b);
int find_neighbour_ports(int id, int dim, int portnum, int neighbours_port[]);

int find_initial_type(int id, int dim);