#include <curses.h>

void usage(char *name);
void get_args(int argc, char **argv, int *dim, int *portnum);
void master_shutdown();