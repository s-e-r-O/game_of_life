int server_init(int portnum);
int wait_for_clients(int server_fd, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state);
