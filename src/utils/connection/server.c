#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connection.h"

int server_init(int portnum)
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    
    /* Creating socket file descriptor in IPv4 <----- LET'S CHANGE THIS */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    /* Forcefully attaching socket to the portnum */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( portnum );
      
    /* Forcefully attaching socket to the portnum */
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
    struct state_msg state_message;
    
    printf("%02d: Listening\n", id);
    
    listen(server_fd, n_needed);

    for (int i = 0; i < n_needed; i++){
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        read(new_socket, &state_message, sizeof(state_message));

        printf("%02d: Connected to %02d\n", id, state_message.id);
        printf("%02d: Received %d from %02d\n", id, state_message.state, state_message.id);
        int pos;
        for (int j = 0; j < n_neighbours; j++){
            if (neighbours[j] == state_message.id){
                pos = j;
                break;
            }
        }
        neighbours_state[pos] = state_message.state;
        state_message.id = id;
        state_message.state = state;

        send(new_socket, &state_message, sizeof(state_message), 0);
        close(new_socket);
    }
}

int wait_for_slaves(int server_fd, int dim, int slaves_state[]){
    
    int new_socket; 
    
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    struct state_msg state_message;

    //printf("M: Listening\n");

    listen(server_fd, dim*dim + 5);
            
    for (int i = 0; i < dim*dim; i++){
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            
        read(new_socket, &state_message, sizeof(state_message));
        close(new_socket);

        //printf("M: Connected to %02d\n", state_message.id);
        //printf("M: Received %d from %02d\n", state_message.state, state_message.id);
        if (slaves_state[state_message.id - 1] != NOT_SET){
            i--;
            continue;
        }
        slaves_state[state_message.id - 1] = state_message.state;
/*
        if (slaves_state[state_message.id - 1] == ALIVE){

            printf("M: Received ALIVE from %02d\n", state_message.id);
        }
*/
    }
}