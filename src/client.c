/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "client.h"

int main(int argc, char* argv[])
{
    char clientRequest[BUFFER_SIZE];
    char messageBuffer[BUFFER_SIZE];
    struct hostent *server;
    struct sockaddr_in serverAddress;
    int socketFileDescriptor;
    int socketMessageSize;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <server_address> <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* hostname = argv[1];
    int portNumber = atoi(argv[2]);

    /* Create socket */
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0)
    {
        fprintf(stderr, "Error: Socket creation error\n");
        exit(EXIT_FAILURE);
    }

    /* Get server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) 
    {
        fprintf(stderr,"Error: No such host with name %s\n", hostname);
        exit(EXIT_FAILURE);
    }

    /* Build server IP address */
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNumber);
    
    if (connect(socketFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
        fprintf(stderr,"Error: TCP Socket Connection Failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Successfully connected to Online Book Catalogue. ");
    printf("Enter a request for a book:\n");

    while (strcmp(messageBuffer, "QUIT\n") != 0 && strcmp(messageBuffer, "quit\n") != 0)
    {
        memset(messageBuffer, '\0', BUFFER_SIZE);

        fgets(messageBuffer, BUFFER_SIZE, stdin);
        strcat(messageBuffer, "\0");

        strcat(clientRequest, messageBuffer);
        strcat(clientRequest, "\0");

        if (strcmp(messageBuffer, "\n") == 0)
        {
            /* Send book request to server */
            socketMessageSize = write(socketFileDescriptor, clientRequest, BUFFER_SIZE);
            if (socketMessageSize < 0)
            {
                fprintf(stderr,"Error: Socket Write Error\n");
                exit(EXIT_FAILURE);
            }

            memset(clientRequest, '\0', BUFFER_SIZE);
            system("clear");
            printf("Client request sent successfully.\n");
            
            /* Read book response from server */
            memset(messageBuffer, '\0', BUFFER_SIZE);
            socketMessageSize = read(socketFileDescriptor, messageBuffer, BUFFER_SIZE);
            if (socketMessageSize < 0)
            {
                fprintf(stderr,"Error: Socket Read Error\n");
                exit(EXIT_FAILURE);
            }

            printf("Server response received successfully.\n");
            printf("SERVER RESPONSE:\n%s", messageBuffer);
            printf("\nEnter a request for a book:\n");
        }
    }
    socketMessageSize = write(socketFileDescriptor, "close", BUFFER_SIZE);
    if (socketMessageSize < 0)
    {
        fprintf(stderr,"Error: Socket Write Error\n");
        exit(EXIT_FAILURE);
    }
    close(socketFileDescriptor);
    printf("Connection terminated successfully\n");
    return 0;
}

/*******************************************************************
* clearString
*
* Terminates a given string at the root
*******************************************************************/
void clearString(char* buffer)
{
	buffer[0] = '\0';
}
