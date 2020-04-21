/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#include <netinet/in.h>
#include "linkedList.h"

#ifndef __SERVER_H__
#define __SERVER_H__

/*******************************************************************
* Client_Thread_Args
*
* Arguments passed threads for multithreaded catalogue processing
*******************************************************************/
typedef struct Client_Thread_Args
{
    Book** bookCatalogue;
    int clientFileDescriptor;
} Client_Thread_Args;

#if 0
/*******************************************************************
* Internet Address
*******************************************************************/
struct in_addr 
{
    unsigned int s_addr; 
};

/*******************************************************************
* Internet style socket address 
*******************************************************************/
struct sockaddr_in  {
    unsigned short int sin_family; /* Address family */
    unsigned short int sin_port;   /* Port number */
    struct in_addr sin_addr;	 /* IP address */
    unsigned char sin_zero[...];   /* Pad to size of 'struct sockaddr' */
};

/*******************************************************************
* Domain name service (DNS) host entry
*******************************************************************/
struct hostent 
{
    char    *h_name;        /* official name of host */
    char    **h_aliases;    /* alias list */
    int     h_addrtype;     /* host address type */
    int     h_length;       /* length of address */
    char    **h_addr_list;  /* list of addresses */
}
#endif

/*******************************************************************
* acceptValidClientConnection
*
* Accepts a valid client connection and returns its file descriptor
*******************************************************************/
int acceptValidClientConnection(int parentFileDescriptor);

/*******************************************************************
* clientHandler
*
* Processes book catalogue requests for a particular client
*******************************************************************/
void* clientHandler(void* param);

/*******************************************************************
* closeValidClientConnection
*
* Closes a valid client connection
*******************************************************************/
int closeValidClientConnection(int childFileDescriptor);

#endif
