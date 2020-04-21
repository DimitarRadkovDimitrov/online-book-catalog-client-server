/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#include "linkedList.h"
#ifndef __BOOKCATALOGUE_H__
#define __BOOKCATALOGUE_H__

static const int BUFFER_SIZE = 1024;
enum STATUS_CODE {OK, MALFORMED_PAYLOAD, SERVER_ERROR};

/*******************************************************************
* clearString
*
* Terminates a given string at the root
*******************************************************************/
void clearString(char* buffer);

/*******************************************************************
* toLowerCase
*
* Given a string changes all characters to lowercase
*******************************************************************/
void toLowerCase(char* str);

/*******************************************************************
* isValidRequestType
*
* Checks if a given string matches any of the known query types
*******************************************************************/
int isValidRequestType(char* type);

/*******************************************************************
* processRequest
*
* Root of request processing. Modifies response string when finished
*******************************************************************/
void processRequest(Book** bookCatalogue, char* request, char* response);

/*******************************************************************
* processSubmitRequest
*
* Adds valid book to catalogue. Appends new book to response
*******************************************************************/
void processSubmitRequest(Book** bookCatalogue, char* requestBody, char* response);

/*******************************************************************
* processGetRequest
*
* Returns list of books found matching query. Appends found books 
* to response
*******************************************************************/
void processGetRequest(Book** bookCatalogue, char* requestBody, char* response);

/*******************************************************************
* processRemoveRequest
*
* Removes valid book from catalogue
*******************************************************************/
void processRemoveRequest(Book** bookCatalogue, char* requestBody, char* response);

/*******************************************************************
* isValidSubmitRequest
*
* Determines if a client request satisfies submit request criteria
*******************************************************************/
int isValidSubmitRequest(char* requestBody);

/*******************************************************************
* isValidGetRequest
*
* Determines if a client request satisfies get request criteria
*******************************************************************/
int isValidGetRequest(char* requestBody);

/*******************************************************************
* isValidRemoveRequest
*
* Determines if a client request satisfies remove request criteria
*******************************************************************/
int isValidRemoveRequest(char* requestBody);

/*******************************************************************
* appendResponseHeader
*
* Based on status code and procedure appends header to the response
*******************************************************************/
void appendResponseHeader(int statusCode, int recordsModified, char* procedure, char* response);

/*******************************************************************
* appendClientRequest
*
* Appends client request header and body to response
*******************************************************************/
void appendClientRequest(char* procedure, char* requestBody, char* response);

/*******************************************************************
* appendExpectedRequestFormat
*
* Based on query type returns request that was expected
*******************************************************************/
void appendExpectedRequestFormat(char* procedure, char* response);

#endif
