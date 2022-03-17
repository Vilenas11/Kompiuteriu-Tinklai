#include "mainHeader.h"
#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void funkcija(int clientSocket);

int main(int argc, char *argv []){
#ifdef _WIN32
    WSADATA data;
#endif
    unsigned int port;
    int l_socket;
    int c_socket; 
    struct sockaddr_in servaddr; 
    struct sockaddr_in clientaddr; 
//    int clientaddrlen;
    socklen_t clientaddrlen;

    int s_len;
    int r_len;
    
    if (argc != 2){
        printf("USAGE: %s <port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);

    if ((port < 1) || (port > 65535)){
        printf("ERROR #1: invalid port specified.\n");
        exit(1);
    }

#ifdef _WIN32
    WSAStartup(MAKEWORD(2,2),&data);    
#endif

    if ((l_socket = socket(AF_INET, SOCK_STREAM,0))< 0){
        fprintf(stderr,"ERROR #2: cannot create listening socket.\n");
        exit(1);
    }
    
    memset(&servaddr,0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // nurodomas protokolas (IP)
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port); // nurodomas portas

    if (bind (l_socket, (struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        fprintf(stderr,"ERROR #3: bind listening socket.\n");
        exit(1);
    }

    if (listen(l_socket, 5) <0){
        fprintf(stderr,"ERROR #4: error in listen().\n");
        exit(1);
    }
    else fprintf(stderr, "Server is listening \n");

        memset(&clientaddr,0, sizeof(clientaddr));

        clientaddrlen = sizeof(struct sockaddr);
        if ((c_socket = accept(l_socket,
            (struct sockaddr*)&clientaddr,&clientaddrlen))<0){
            fprintf(stderr,"ERROR #5: error occured accepting connection.\n");
            exit(1);
        }
        funkcija(c_socket);

    return 0;
}
void funkcija(int clientSocket){
    char buffer[1024];
    for(;;){
      int k;
        memset(&buffer,0,sizeof(buffer));
        k = recv(clientSocket,buffer,sizeof(buffer),0);
        buffer[strlen(buffer)-1] = '\0';
        printf("%s,%d\n", buffer, k);
        if(k == -1) {
            printf("The client has disconnected\n");
            close(clientSocket);
            printf("Closing the server");
            break;
        }
    }

}