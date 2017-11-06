#include <curses.h>

void usage(char *name);
void get_args(int argc, char **argv, int *dim, int *portnum);

// NCURSES oriented functions

void master_shutdown();
void show_generation(WINDOW *win, int gen);
void show_slaves(WINDOW *win, int dim, int states[]);