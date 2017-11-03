/**
    Handle multiple socket connections with select and fd_set on Linux
*/
  
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
  
#define TRUE   1
#define FALSE  0
#define PORT 8888
 
int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket,master_socket6 , 
    addrlen,addrlen6 , new_socket , client_socket[30] , max_clients = 30 , activity,valread ,activity6,valread6 , sd;
    int max_sd;
    struct sockaddr_in address;
    struct sockaddr_in6 address6;
      
    char buffer[1025];  //data buffer of 1K
      
    //set of socket descriptors
    fd_set readfds;
      
    //a message
    char *message = "Oliwis Chikibaibis \r\n";
  
    //initialise all client_socket[] to 0 so not checked
    for (int i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
      
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0 || (master_socket6 = socket(AF_INET6 , SOCK_STREAM , 0)) == 0 ) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 || 
    	setsockopt(master_socket6, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    };
  
    //type of socket created
    address.sin_family = AF_INET; //AF_INET6 for ipv6
    //address.sin_addr.s_addr = //INADDR_ANY;
    address.sin_port = htons( PORT );
    if(inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr))<=0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    //Socket6Creation
    //type of socket created
    address6.sin6_family = AF_INET6; //AF_INET6 for ipv6
    address6.sin6_addr = in6addr_any;
    address6.sin6_port = htons( PORT );
      if(inet_pton(AF_INET6, "::1", &(address6.sin6_addr))<=0) {
        perror("inet_pton6 failed");
        exit(EXIT_FAILURE);
    }  
    //bind the socket to localhost port 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        printf("bind ipv4 ports\n");
        exit(EXIT_FAILURE);
    }
    printf("binded ipv4\n");
    //printf("Listener on port %d \n", PORT);

    if ( bind(master_socket6, (struct sockaddr *)&address6, sizeof(address6))<0){
    	printf("bind ipv6 ports\n");
    	exit(EXIT_FAILURE);
    } 
    printf("binded ipv6\n");
     
    //try to specify maximum of 100 pending connections for the master socket
    if (listen(master_socket, 3) < 0 || listen(master_socket6, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    addrlen = sizeof(address);
    addrlen6 = sizeof(address6);
    printf("Waiting for connections ...\n");
     
    while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add masters socket to set
        FD_SET(master_socket, &readfds);
        FD_SET(master_socket6, &readfds);
        max_sd = master_socket6;
         
        //add child sockets to set
        for (int i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        //activity6 = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
              
            printf("Welcome message sent successfully\n");
              
            //add new socket to array of sockets
            for (int i = 0; i < max_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
          
            //inform user of socket number - used in send and receive command
            
        }

        if (FD_ISSET(master_socket6, &readfds)) 
        {
            if ((new_socket = accept(master_socket6, (struct sockaddr *)&address6, (socklen_t*)&addrlen6))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            char straddr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &address6.sin6_addr, straddr, sizeof(straddr));
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , straddr , ntohs(address6.sin6_port));
        
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
              
            printf("Welcome message sent successfully\n");
              
            //add new socket to array of sockets
            for (int i = 0; i < max_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (int i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                  
                //Doing Something With The message recieved
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    printf ("%s",buffer);
                    send(sd , "Llego tu mensaje Churribombon \n" , strlen("Llego tu mensaje Churribombon \n") , 0 );
                    
                }
            }
        }
    }
      
    return 0;
} 