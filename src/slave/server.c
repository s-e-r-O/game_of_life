//connection try
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.h"

int server(int portnum)
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