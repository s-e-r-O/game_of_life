#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client.h"
#include "utils.h"
  

int connect_to_servers(int init_portnum, int n_needed, int n_neighbours, int neighbours[], int neighbours_state[], int id, int state){
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    

    int connection_success = 0;    
    while(connection_success != n_needed){
        sleep(1);
        for (int i = 0; i < n_neighbours; i++){
            
            if (neighbours_state[i] != NOT_SET){
                continue;
            }

            int sock = socket(AF_INET, SOCK_STREAM, 0);

            serv_addr.sin_port = htons(init_portnum + neighbours[i]);
            if (id == 0) printf("%02d: Connecting to %02d in port %d\n", id, neighbours[i], init_portnum + neighbours[i]);
            if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                continue;
            }
            
            connection_success++;
            if (id == 0) printf("%02d: Success connecting to %02d\n", id, neighbours[i]);
            
            struct state_msg state_message;
            state_message.id = id;
            state_message.state = state;
            
            send(sock, &state_message, sizeof(state_message), 0);

            read(sock, &state_message, sizeof(state_message));

            if (id == 0) printf("%02d: Received %d from %02d\n", id, state_message.state, state_message.id);
            for (int j = 0; j < n_neighbours; j++){
                if (neighbours[j] == state_message.id){
                    neighbours_state[j] = state_message.state;
                    break;
                }
            }
        }
    }
}