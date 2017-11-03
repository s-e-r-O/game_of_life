#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connection.h"

/* WE DEFINETLY HAVE TO CHANGE THIS TO IMPLEMENT IPv4 AND IPv6 */
int address_init(struct sockaddr_in *address){
    address->sin_family = AF_INET;
    
    /* Localhost address */
    if(inet_pton(AF_INET, "127.0.0.1", &(address->sin_addr))<=0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

}

int connect_to_servers(int init_portnum, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state){
    
    struct state_msg state_message;
    
    struct sockaddr_in serv_addr;
    address_init(&serv_addr);
    
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

            int sock = socket(AF_INET, SOCK_STREAM, 0); /* ANOTHER CHANGE HERE */

            int neighbour_port = init_portnum + neighbours[i];
            serv_addr.sin_port = htons(neighbour_port);
            
            printf("%02d: Trying to connect to %02d in port %d\n", id, neighbours[i], neighbour_port);
            
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                continue;
            }
            
            connection_success++;
            
            printf("%02d: Success connecting to %02d\n", id, neighbours[i]);
            
            state_message.id = id;
            state_message.state = state;
            
            send(sock, &state_message, sizeof(state_message), 0);
            read(sock, &state_message, sizeof(state_message));

            printf("%02d: Received %d from %02d\n", id, state_message.state, state_message.id);
            
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
    address_init(&serv_addr);
    
    int sock = socket(AF_INET, SOCK_STREAM, 0); /* ANOTHER CHANGE HERE */

    serv_addr.sin_port = htons(init_portnum);
    printf("%02d: Trying to connect to master\n", id);

    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0);
    
    printf("%02d: Success connecting to master\n", id);

    state_message.id = id;
    state_message.state = state;
    
    send(sock, &state_message, sizeof(state_message), 0);   
}