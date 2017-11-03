#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

  
int main(int argc, char const *argv[])
{
    int port;
    
    if (argc > 1){
        port = atoi(argv[1]);
    } else {
        printf("Usage:\n");
        printf("    client portnum\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in6 address;
    int sock = 0, valread;
    struct sockaddr_in6 serv_addr;
    
    char *hello = "Hello from client\n";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    //memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(port);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET6, "::1", &serv_addr.sin6_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed   Retrying \n");
    }
    
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}