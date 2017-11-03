//connection try
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.h"
#include "utils.h"

int server_init(int portnum)
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
      
    // Creating socket file descriptor in IPV4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the portnum
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( portnum );
      
    // Forcefully attaching socket to the portnum
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int wait_for_clients(int server_fd, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state){
    
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    printf("%02d: Listening\n", id);
    listen(server_fd, n_neighbours);

    for (int i = 0; i < n_needed; i++){
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        int peer_id;
        struct state_msg state_message;
        read(new_socket, &state_message, sizeof(state_message));
        if (id == 0) printf("%02d: Connected to %02d\n", id, state_message.id);
        if (id == 0) printf("%02d: Received %d from %02d\n", id, state_message.state, state_message.id);
        for (int j = 0; j < n_neighbours; j++){
            if (neighbours[j] == state_message.id){
                neighbours_state[j] = state_message.state;
                break;
            }
        }
        state_message.id = id;
        state_message.state = state;

        send(new_socket, &state_message, sizeof(state_message), 0);
        //printf("Connected to %d! my n_neighbours is %d\n", state_message.id, n_neighbours);
        //printf("%02d: Connected to %02d (Port: %d)...\n", id, peer_id, address.sin_port);
    }
}