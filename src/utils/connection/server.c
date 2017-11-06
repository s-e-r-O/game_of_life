#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>

#include "connection.h"

int server_init(int portnum, int type)
{
    int server_fd;
    struct sockaddr_in address;
    struct sockaddr_in6 address6;
    int opt = 1;
    
    /* Creating socket file descriptor in IPv4 or IPv6 */
    if (type == 4)
    {
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
    }
    if (type == 6)
    {
        if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }   
    }
      
    /* Forcefully attaching socket to the portnum */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET; //AF_INET6 for ipv6
    //address.sin_addr.s_addr = //INADDR_ANY;
    address.sin_port = htons( portnum );
    if(inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr))<=0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    address6.sin6_family = AF_INET6; //AF_INET6 for ipv6
    //address6.sin6_addr = in6addr_any;
    address6.sin6_port = htons( portnum );
      if(inet_pton(AF_INET6, "::1", &(address6.sin6_addr))<=0) {
        perror("inet_pton6 failed");
        exit(EXIT_FAILURE);
    }
      
    /* Forcefully attaching socket to the portnum */
    if (type == 4)
    {
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }

    if (type == 6)
    {
        if (bind(server_fd, (struct sockaddr *) &address6, sizeof(address6))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }
    return server_fd;
}

int wait_for_clients(int server_fd,int server_fd6, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state){
    
    int new_socket;
    struct sockaddr_in address;
    struct sockaddr_in6 address6;
    int addrlen = sizeof(address);
    int addrlen6 = sizeof(address6);
    struct state_msg state_message;
    int activity, max_sd;
    fd_set readfds;
    
    printf("%02d: Listening\n", id);
    
    listen(server_fd, n_needed);
    listen(server_fd6, n_needed);

    FD_ZERO(&readfds);

    FD_SET(server_fd, &readfds);
    FD_SET(server_fd6, &readfds);
    max_sd = server_fd6;

    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        //hacer Un IF con el FD_ISSET de ipv6-caso contrario FD_ISSET de ipv4
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
        
    for (int i = 0; i < n_needed; i++){
        



        if (FD_ISSET(server_fd, &readfds)) 
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
                
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
            
        }

        else if (FD_ISSET(server_fd6, &readfds)) 
        {
            if ((new_socket = accept(server_fd6, (struct sockaddr *)&address6, (socklen_t*)&addrlen6))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
                
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
        }
        close(new_socket);
    }
}

int wait_for_slaves(int server_fd,int server_fd6, int dim, int slaves_state[]){
    
    int new_socket; 
    
    struct sockaddr_in address;
    struct sockaddr_in6 address6;
    int addrlen = sizeof(address);
    int addrlen6 = sizeof(address6);
    int activity, max_sd;
    fd_set readfds;

    struct state_msg state_message;

    //printf("M: Listening\n");

    listen(server_fd, dim*dim );
    listen(server_fd6, dim*dim );
    

    FD_ZERO(&readfds);

    FD_SET(server_fd, &readfds);
    FD_SET(server_fd6, &readfds);
    max_sd = server_fd6;

    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        //hacer Un IF con el FD_ISSET de ipv6-caso contrario FD_ISSET de ipv4
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }

    for (int i = 0; i < dim*dim; i++){




        if (FD_ISSET(server_fd, &readfds)) 
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
                
            read(new_socket, &state_message, sizeof(state_message));

            if (slaves_state[state_message.id - 1] != NOT_SET){
                close(new_socket);
                i--;
                continue;
            }
            slaves_state[state_message.id - 1] = state_message.state;
            
        }

        else if (FD_ISSET(server_fd6, &readfds)) 
        {
            if ((new_socket = accept(server_fd6, (struct sockaddr *)&address6, (socklen_t*)&addrlen6))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
                
            read(new_socket, &state_message, sizeof(state_message));

            if (slaves_state[state_message.id - 1] != NOT_SET){
            i--;
            close(new_socket);
            continue;
            }
            slaves_state[state_message.id - 1] = state_message.state;
            
        }
       
        close(new_socket);

        //printf("M: Connected to %02d\n", state_message.id);
        //printf("M: Received %d from %02d\n", state_message.state, state_message.id);
        
/*
        if (slaves_state[state_message.id - 1] == ALIVE){

            printf("M: Received ALIVE from %02d\n", state_message.id);
        }
*/
    }
}