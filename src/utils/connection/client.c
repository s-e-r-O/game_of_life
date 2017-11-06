#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connection.h"

int address4_init(struct sockaddr_in *address){
    address->sin_family = AF_INET;
    
    /* Localhost address */
    if(inet_pton(AF_INET, "127.0.0.1", &(address->sin_addr))<=0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

}

int address6_init(struct sockaddr_in6 *address){
    address->sin6_family = AF_INET6;
    
    /* Localhost address */
    if(inet_pton(AF_INET6, "::1", &(address->sin6_addr))<=0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

}

int connect_to_servers(int init_portnum, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state){
    
    struct state_msg state_message;
    
    struct sockaddr_in serv_addr;
    address4_init(&serv_addr);

    struct sockaddr_in6 serv_addr6;
    address6_init(&serv_addr6);
    
    /* Keeps track of how many servers we could make a connection with */
    int connection_success = 0; 
    
    while(connection_success != n_needed){
        /* Let's give some time to the servers */
        sleep(1);

        /* We are trying to reach all of our neighbours, even if they aren't servers */
        for (int i = 0; i < n_neighbours; i++){
            
            /* We don't try to connect to a neighbour if we already have its state */
            if (neighbours_state[i] != NOT_SET){
                continue;
            }

            int neighbour_port = init_portnum + neighbours[i];

            /* We store the socket in which me made a connection (IPv4 or IPv6)*/
            int connected_socket;

            /* Let's try first with IPv6 */
            int sock6 = socket(AF_INET6, SOCK_STREAM, 0);
            serv_addr6.sin6_port = htons(neighbour_port);

            printf("%02d: Trying to connect to %02d with IPv6 in port %d\n", id, neighbours[i], neighbour_port);

            if (connect(sock6, (struct sockaddr *)&serv_addr6, sizeof(serv_addr6)) < 0) {
                close(sock6); /* In case it failed, we close the socket for IPv6 connection, and try to connect with IPv4 */
                int sock = socket(AF_INET, SOCK_STREAM, 0); 

                serv_addr.sin_port = htons(neighbour_port);

                printf("%02d: Trying to connect to %02d with IPv4 in port %d\n", id, neighbours[i], neighbour_port);
                
                if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                    /* If it failed, it means that the neighbour isn't a server, so we move on */
                    close(sock);
                    continue;
                } else {
                    connected_socket = sock;
                }
            } else {
                connected_socket = sock6;
            }
            
            connection_success++;
            
            printf("%02d: Success connecting to %02d\n", id, neighbours[i]);
            
            state_message.id = id;
            state_message.state = state;
            
            send(connected_socket, &state_message, sizeof(state_message), 0);
            read(connected_socket, &state_message, sizeof(state_message));

            close(connected_socket);
            printf("%02d: Received %d from %02d\n", id, state_message.state, state_message.id);
            
            /* We store the state of the connected neighbour */
            for (int j = 0; j < n_neighbours; j++){
                if (neighbours[j] == state_message.id){
                    neighbours_state[j] = state_message.state;
                    break;
                }
            }
        }
    }
}

int connect_to_master(int init_portnum, int id, int state){
    
    struct state_msg state_message;

    struct sockaddr_in serv_addr;    
    address4_init(&serv_addr);

    struct sockaddr_in6 serv_addr6;
    address6_init(&serv_addr6);
    
    int connected_socket;

    int sock6 = socket(AF_INET6, SOCK_STREAM, 0);
    serv_addr6.sin6_port = htons(init_portnum);


    printf("%02d: Trying to connect to master\n", id);
    int connected = 0;
    /* Giving that the master is always server, we should be able to reach it all the time */
    while (!connected){
        /* Trying first with IPv6 */
        if (connect(sock6, (struct sockaddr *)&serv_addr6, sizeof(serv_addr6)) < 0){
            close(sock6); /* In case it failed, we close the socket for IPv6 connection, and try to connect with IPv4 */
            int sock = socket(AF_INET, SOCK_STREAM, 0); 

            serv_addr.sin_port = htons(init_portnum);

            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0){
                connected_socket = sock;
                connected = 1;
            } else {
                close(sock);
            }
        } else {
            connected_socket = sock6;
            connected = 1;
        }
        
    }
    
    printf("%02d: Success connecting to master\n", id);

    state_message.id = id;
    state_message.state = state;
    
    send(connected_socket, &state_message, sizeof(state_message), 0); 

    close(connected_socket);  
}