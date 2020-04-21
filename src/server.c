/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "server.h"
#include "bookCatalogue.h"
#include "linkedList.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Book* bookCatalogue = NULL;

int main(int argc, char* argv[])
{
    pthread_t thread_id;
    int portNumber;
    int optval; 
    int parentFileDescriptor;
    int childFileDescriptor;
    struct sockaddr_in serverAddress; 

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    portNumber = atoi(argv[1]);

    /* Create parent socket */
    parentFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (parentFileDescriptor < 0) 
    {
        fprintf(stderr, "Error: Parent socket creation error\n");
        exit(EXIT_FAILURE);
    }

    optval = 1;
    setsockopt(parentFileDescriptor, SOL_SOCKET, SO_REUSEADDR, 
        (const void *)&optval , sizeof(int));

    /* Build server IP address */
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons((unsigned short) portNumber);

    /* Associate parent socket with port */
    if (bind(parentFileDescriptor, (struct sockaddr *) &serverAddress, 
    sizeof(serverAddress)) < 0) 
    {
        fprintf(stderr, "Error: Parent socket port binding error\n");
        exit(EXIT_FAILURE);
    }
    printf("Parent socket has successfully binded to port.\n");

    /* Socket ready to accept 5 connection requests */
    if (listen(parentFileDescriptor, 5) < 0)
    {
        fprintf(stderr, "Error: Error on listen\n");
        exit(EXIT_FAILURE);
    }

    /* Main loop for accepting client connections */
    while (1)
    {
        childFileDescriptor = acceptValidClientConnection(parentFileDescriptor);
        int pthreadResult = pthread_create(&thread_id, NULL, clientHandler, (void*) &childFileDescriptor);

        if (pthreadResult != 0)
        {
            fprintf(stderr, "Error: Error on pthread_create\n");
            exit(EXIT_FAILURE);
        }
    }

    pthread_exit(NULL);
    destroyBookList(bookCatalogue);
    return 0;
}

/*******************************************************************
* acceptValidClientConnection
*
* Accepts a valid client connection and returns its file descriptor
*******************************************************************/
int acceptValidClientConnection(int parentFileDescriptor)
{
    struct sockaddr_in clientAddress;
    struct hostent *hostp;
    int childFileDescriptor;
    char *hostaddrp;
    int clientAddressLen = sizeof(clientAddress);

    childFileDescriptor = accept(parentFileDescriptor, (struct sockaddr *) &clientAddress, (socklen_t *) &clientAddressLen);
    if (childFileDescriptor < 0) 
    {
        fprintf(stderr, "Error: Error on accept\n");
        exit(EXIT_FAILURE);
    }

    hostp = gethostbyaddr((const char *)&clientAddress.sin_addr.s_addr, 
            sizeof(clientAddress.sin_addr.s_addr), AF_INET);

    if (hostp == NULL)
    {
        fprintf(stderr, "Error: Error getting host address\n");
        exit(EXIT_FAILURE);
    }

    hostaddrp = inet_ntoa(clientAddress.sin_addr);
    if (hostaddrp == NULL)
    {
        fprintf(stderr, "Error: Error on inet_ntoa\n");
        exit(EXIT_FAILURE);
    }

    printf("Server established connection with %s (%s).\n", hostp->h_name, hostaddrp);
    return childFileDescriptor;    
}

/*******************************************************************
* clientHandler
*
* Processes book catalogue requests for a particular client
*******************************************************************/
void* clientHandler(void* param)
{
    int childFileDescriptor = *((int*) param);
    char messageBuffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int socketMessageSize;

    while (1)
    {
        clearString(response);
        clearString(messageBuffer);
        socketMessageSize = read(childFileDescriptor, messageBuffer, BUFFER_SIZE);

        if (socketMessageSize < 0) 
        {
            fprintf(stderr, "Error: Socket read error\n");
            exit(EXIT_FAILURE);
        }
        printf("Client request received successfully.\n");

        if (strcmp(messageBuffer, "close") == 0)
        {
            break;
        }

        pthread_mutex_lock(&mutex);
        processRequest(&bookCatalogue, messageBuffer, response);
        pthread_mutex_unlock(&mutex);

        clearString(messageBuffer);

        strcpy(messageBuffer, response);
        strcat(messageBuffer, "\0");

        socketMessageSize = write(childFileDescriptor, messageBuffer, strlen(messageBuffer));
        if (socketMessageSize < 0) 
        {
            fprintf(stderr, "Error: Socket write error\n");
            exit(EXIT_FAILURE);
        }
        printf("Server response delivered successfully.\n");
    }
    closeValidClientConnection(childFileDescriptor);
    return NULL;
}

/*******************************************************************
* closeValidClientConnection
*
* Closes a valid client connection
*******************************************************************/
int closeValidClientConnection(int childFileDescriptor)
{    
    int result = close(childFileDescriptor);
    printf("Client connection closed.\n");
    return result;
}
