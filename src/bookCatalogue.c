/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bookCatalogue.h"
#include "linkedList.h"

/*******************************************************************
* clearString
*
* Terminates a given string at the root
*******************************************************************/
void clearString(char* buffer)
{
	buffer[0] = '\0';
}

/*******************************************************************
* toLowerCase
*
* Given a string changes all characters to lowercase
*******************************************************************/
void toLowerCase(char* str)
{
    if (str != NULL)
    {
        for (int i = 0; str[i]; i++)
        {
            str[i] = tolower(str[i]);
        }
    }  
}

/*******************************************************************
* isValidRequestType
*
* Checks if a given string matches any of the known query types
*******************************************************************/
int isValidRequestType(char* type)
{
    if (type != NULL)
    {
        toLowerCase(type);
        if (strcmp(type, "get") == 0 || strcmp(type, "remove") == 0 || strcmp(type, "submit") == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*******************************************************************
* processRequest
*
* Root of request processing. Modifies response string when finished
*******************************************************************/
void processRequest(Book** bookCatalogue, char* request, char* response)
{
    char requestCopy[BUFFER_SIZE];
    char requestHeader[BUFFER_SIZE];
    char requestBody[BUFFER_SIZE];
    char* requestLine;

    strcpy(requestCopy, request);
    strcat(requestCopy, "\0");

    clearString(response);
    requestLine = strtok(requestCopy, "\n");

    if (requestLine != NULL)
    {
        clearString(requestBody);
        strcpy(requestHeader, requestLine);
        strcat(requestHeader, "\0");
        
        if (!isValidRequestType(requestHeader))
        {
            appendResponseHeader(MALFORMED_PAYLOAD, 0, NULL, response);
            return;
        }

        requestLine = strtok(NULL, "\n");
        while (requestLine != NULL)
        {
            strcat(requestBody, requestLine);
            strcat(requestBody, "\n");
            requestLine = strtok(NULL, "\n");
        }
        strcat(requestBody, "\0");

        if (strcmp(requestBody, "") != 0)
        {
            if (strcmp(requestHeader, "submit") == 0)
            {  
                processSubmitRequest(&(*bookCatalogue), requestBody, response);
            }
            else if (strcmp(requestHeader, "get") == 0)
            {
                processGetRequest(&(*bookCatalogue), requestBody, response);
            }
            else if (strcmp(requestHeader, "remove") == 0)
            {
                processRemoveRequest(&(*bookCatalogue), requestBody, response);
            }
        }
        else
        {
            appendResponseHeader(MALFORMED_PAYLOAD, 0, NULL, response);
        }
    }
}

/*******************************************************************
* processSubmitRequest
*
* Adds valid book to catalogue. Appends new book to response
*******************************************************************/
void processSubmitRequest(Book** bookCatalogue, char* requestBody, char* response)
{
    char* procedureName = "submit\0";
    char requestBodyCopy[BUFFER_SIZE];
    char titleValue[BUFFER_SIZE];
    char authorValue[BUFFER_SIZE];
    char locationValue[BUFFER_SIZE];
    int numRecordsUpdated = 0;
    char* fieldValueToken;

    if (!isValidSubmitRequest(requestBody))
    {
        appendResponseHeader(MALFORMED_PAYLOAD, numRecordsUpdated, procedureName, response);
        appendClientRequest(procedureName, requestBody, response);
        appendExpectedRequestFormat(procedureName, response);
        return;
    }
    
    strcpy(requestBodyCopy, requestBody);
    strcat(requestBodyCopy, "\0");

    fieldValueToken = strtok(requestBodyCopy, " ");
    fieldValueToken = strtok(NULL, "\n");

    strcpy(titleValue, fieldValueToken);
    strcat(titleValue, "\0");

    fieldValueToken = strtok(NULL, " ");
    fieldValueToken = strtok(NULL, "\n");

    strcpy(authorValue, fieldValueToken);
    strcat(authorValue, "\0");

    fieldValueToken = strtok(NULL, " ");
    fieldValueToken = strtok(NULL, "\n");

    strcpy(locationValue, fieldValueToken);
    strcat(locationValue, "\0");

    Book* newBook = createBook(titleValue, authorValue, locationValue);
    (*bookCatalogue) = addBookToBack((*bookCatalogue), newBook, &numRecordsUpdated); 

    appendResponseHeader(OK, numRecordsUpdated, procedureName, response);
    appendBookToString(newBook, response);
}

/*******************************************************************
* processGetRequest
*
* Returns list of books found matching query. Appends found books 
* to response
*******************************************************************/
void processGetRequest(Book** bookCatalogue, char* requestBody, char* response)
{
    char* procedureName = "get\0";
    char requestBodyCopy[BUFFER_SIZE];
    char titleValue[BUFFER_SIZE];
    char authorValue[BUFFER_SIZE];
    int numRecordsFound = 0;
    char* fieldValueToken;
    Book* searchResults = NULL;
    
    if (!isValidGetRequest(requestBody))
    {
        appendResponseHeader(MALFORMED_PAYLOAD, numRecordsFound, procedureName, response);
        appendClientRequest(procedureName, requestBody, response);
        appendExpectedRequestFormat(procedureName, response);
        return;
    }

    strcpy(requestBodyCopy, requestBody);
    strcat(requestBodyCopy, "\0");

    fieldValueToken = strtok(requestBodyCopy, " ");
    toLowerCase(fieldValueToken);

    if (strcmp(fieldValueToken, "title") == 0)
    {
        fieldValueToken = strtok(NULL, "\n");
        strcpy(titleValue, fieldValueToken);
        strcat(titleValue, "\0");

        fieldValueToken = strtok(NULL, " ");
        if (fieldValueToken != NULL)
        {
            fieldValueToken = strtok(NULL, "\n");
            strcpy(authorValue, fieldValueToken);
            strcat(authorValue, "\0");
            searchResults = findBooks((*bookCatalogue), titleValue, authorValue, &numRecordsFound);
        }
        else
        {
            searchResults = findBooks((*bookCatalogue), titleValue, NULL, &numRecordsFound);
        }
    }
    else if (strcmp(fieldValueToken, "author") == 0)
    {
        fieldValueToken = strtok(NULL, "\n");
        strcpy(authorValue, fieldValueToken);
        strcat(authorValue, "\0");
        searchResults = findBooks((*bookCatalogue), NULL, authorValue, &numRecordsFound);
    }

    appendResponseHeader(OK, numRecordsFound, procedureName, response);

    if (searchResults != NULL)
    {
        appendBookToString(searchResults, response);
        destroyBookList(searchResults);
    }
}

/*******************************************************************
* processRemoveRequest
*
* Removes valid book from catalogue
*******************************************************************/
void processRemoveRequest(Book** bookCatalogue, char* requestBody, char* response)
{
    char* procedureName = "remove\0";
    char requestBodyCopy[BUFFER_SIZE];
    char titleValue[BUFFER_SIZE];
    char authorValue[BUFFER_SIZE];
    int numRecordsRemoved = 0;
    char* fieldValueToken;

    if (!isValidRemoveRequest(requestBody))
    {
        appendResponseHeader(MALFORMED_PAYLOAD, numRecordsRemoved, procedureName, response);
        appendClientRequest(procedureName, requestBody, response);
        appendExpectedRequestFormat(procedureName, response);
        return;
    }

    strcpy(requestBodyCopy, requestBody);
    strcat(requestBodyCopy, "\0");

    fieldValueToken = strtok(requestBodyCopy, " ");
    fieldValueToken = strtok(NULL, "\n");

    strcpy(titleValue, fieldValueToken);
    strcat(titleValue, "\0");

    fieldValueToken = strtok(NULL, " ");
    fieldValueToken = strtok(NULL, "\n");

    strcpy(authorValue, fieldValueToken);
    strcat(authorValue, "\0");

    (*bookCatalogue) = removeBook((*bookCatalogue), titleValue, authorValue, &numRecordsRemoved);
    appendResponseHeader(OK, numRecordsRemoved, procedureName, response);
}

/*******************************************************************
* isValidSubmitRequest
*
* Determines if a client request satisfies submit request criteria
*******************************************************************/
int isValidSubmitRequest(char* requestBody)
{
    char* lineToken;
    char requestCopy[BUFFER_SIZE];
    
    strcpy(requestCopy, requestBody);
    strcat(requestCopy, "\0");
    lineToken = strtok(requestCopy, " ");
    toLowerCase(lineToken);

    if (lineToken != NULL && strcmp(lineToken, "title") == 0)
    {
        lineToken = strtok(NULL, "\n");

        if (lineToken != NULL)
        {
            lineToken = strtok(NULL, " ");
            toLowerCase(lineToken);

            if (lineToken != NULL && strcmp(lineToken, "author") == 0)
            {
                lineToken = strtok(NULL, "\n");

                if (lineToken != NULL)
                {
                    lineToken = strtok(NULL, " ");
                    toLowerCase(lineToken);

                    if (lineToken != NULL && strcmp(lineToken, "location") == 0)
                    {
                        lineToken = strtok(NULL, "\n");
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/*******************************************************************
* isValidGetRequest
*
* Determines if a client request satisfies get request criteria
*******************************************************************/
int isValidGetRequest(char* requestBody)
{
    char* lineToken;
    char requestCopy[BUFFER_SIZE];
    
    strcpy(requestCopy, requestBody);
    strcat(requestCopy, "\0");
    lineToken = strtok(requestCopy, " ");

    if (lineToken != NULL)
    {
        toLowerCase(lineToken);
        if (strcmp(lineToken, "title") == 0)
        {
            lineToken = strtok(NULL, "\n");
            if (lineToken != NULL)
            {
                lineToken = strtok(NULL, " ");
                if (lineToken != NULL)
                {
                    toLowerCase(lineToken);
                    if (strcmp(lineToken, "author") == 0)
                    {
                        lineToken = strtok(lineToken, "\n");
                        if (lineToken != NULL)
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
        else if (strcmp(lineToken, "author") == 0)
        {
            lineToken = strtok(NULL, "\n");
            if (lineToken != NULL)
            {
                return 1;
            }
        }
    }
    return 0;
}

/*******************************************************************
* isValidRemoveRequest
*
* Determines if a client request satisfies remove request criteria
*******************************************************************/
int isValidRemoveRequest(char* requestBody)
{
    char* lineToken;
    char requestCopy[BUFFER_SIZE];
    
    strcpy(requestCopy, requestBody);
    strcat(requestCopy, "\0");
    lineToken = strtok(requestCopy, " ");
    toLowerCase(lineToken);

    if (lineToken != NULL && strcmp(lineToken, "title") == 0)
    {
        lineToken = strtok(NULL, "\n");

        if (lineToken != NULL)
        {
            lineToken = strtok(NULL, " ");
            toLowerCase(lineToken);

            if (lineToken != NULL && strcmp(lineToken, "author") == 0)
            {
                lineToken = strtok(NULL, "\n");

                if (lineToken != NULL)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/*******************************************************************
* appendResponseHeader
*
* Based on status code and procedure appends header to the response
*******************************************************************/
void appendResponseHeader(int statusCode, int recordsModified, char* procedure, char* response)
{
    char responseHeader[BUFFER_SIZE];
    char recordsModifiedBuffer[BUFFER_SIZE];

    if (statusCode == OK)
    {
        strcpy(responseHeader, "100 OK\0");
    }
    else if (statusCode == MALFORMED_PAYLOAD)
    {
        strcpy(responseHeader, "400 MALFORMED PAYLOAD\0");
    }
    else if (statusCode == SERVER_ERROR)
    {
        strcpy(responseHeader, "600 Server Error\0");
    }

    sprintf(recordsModifiedBuffer, "%d", recordsModified);
    strcat(recordsModifiedBuffer, "\0");

    if (procedure != NULL)
    {
        strcat(responseHeader, " - ");
        strcat(responseHeader, recordsModifiedBuffer);
        strcat(responseHeader, " ");

        if (strcmp(procedure, "submit") == 0)
        {
            strcat(responseHeader, "Record(s) Updated\n");
        }
        else if (strcmp(procedure, "get") == 0)
        {
            strcat(responseHeader, "Record(s) Found\n");
        }
        else if (strcmp(procedure, "remove") == 0)
        {
            strcat(responseHeader, "Record(s) Removed\n");
        }
        strcat(responseHeader, "\0");
    }
    else
    {
        strcat(responseHeader, " - 0 Record(s) Modified\n\0");
    }
    strcat(response, responseHeader);
}

/*******************************************************************
* appendClientRequest
*
* Appends client request header and body to response
*******************************************************************/
void appendClientRequest(char* procedure, char* requestBody, char* response)
{
    char receivedRequest[BUFFER_SIZE];
    strcpy(receivedRequest, "\nPayload Received:\n");
    
    if (strcmp(procedure, "submit") == 0)
    {
        strcat(receivedRequest, "SUBMIT\n");
        
    }
    else if (strcmp(procedure, "get") == 0)
    {
        strcat(receivedRequest, "GET\n");
        
    }
    else if (strcmp(procedure, "remove") == 0)
    {
        strcat(receivedRequest, "REMOVE\n");
        
    }
    strcat(receivedRequest, requestBody);
    strcat(receivedRequest, "\0");
    strcat(response, receivedRequest);
}

/*******************************************************************
* appendExpectedRequestFormat
*
* Based on query type returns request that was expected
*******************************************************************/
void appendExpectedRequestFormat(char* procedure, char* response)
{
    char responseExpected[BUFFER_SIZE];
    strcpy(responseExpected, "\nExpected:\n");

    if (strcmp(procedure, "submit") == 0)
    {
        strcat(responseExpected, "SUBMIT\n");
        strcat(responseExpected, "TITLE_{Book Title}\n");
        strcat(responseExpected, "AUTHOR_{Book Author}\n");
        strcat(responseExpected, "TITLE_{Book Location}\n\n");
    }
    else if (strcmp(procedure, "get") == 0)
    {
        strcat(responseExpected, "(1)\nGET\n");
        strcat(responseExpected, "TITLE_{Book Title}\n\n");
        strcat(responseExpected, "OR\n\n(2)\n");
        strcat(responseExpected, "GET\nAUTHOR_{Book Author}\n\n");
        strcat(responseExpected, "OR\n\n(3)\n");
        strcat(responseExpected, "GET\nTITLE_{Book Title}\n");
        strcat(responseExpected, "AUTHOR_{Book Author}\n\n");
    }
    else if (strcmp(procedure, "remove") == 0)
    {
        strcat(responseExpected, "REMOVE\n");
        strcat(responseExpected, "TITLE_{Book Title}\n");
        strcat(responseExpected, "AUTHOR_{Book Author}\n\n");
    }
    strcat(responseExpected, "\0");
    strcat(response, responseExpected);
}
