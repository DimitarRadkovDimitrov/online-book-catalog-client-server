/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

/*******************************************************************
* createBook
*
* Creates a book entity using dynamic memory and fields provided
*******************************************************************/
Book* createBook(char* title, char* author, char* location)
{
    Book* book = malloc(sizeof(Book));
    book->title = malloc(sizeof(char) * (strlen(title) + 1));
    book->author = malloc(sizeof(char) * (strlen(author) + 1));
    book->location = malloc(sizeof(char) * (strlen(location) + 1));

    if (book != NULL)
    {
        strcpy(book->title, title);
        strcpy(book->author, author);
        strcpy(book->location, location);
        book->next = NULL;
        return book;
    }

    printf("Malloc failed\n");
    return NULL;
}

/*******************************************************************
* equals
*
* Given two books determines if they have identical fields
*******************************************************************/
int equals(Book* bookOne, Book* bookTwo)
{
    if (bookOne == NULL || bookTwo == NULL)
    {
        return 0;
    }
    return (strcmp(bookOne->title, bookTwo->title) == 0 
        && strcmp(bookOne->author, bookTwo->author) == 0
        && strcmp(bookOne->location, bookTwo->location) == 0);
}

/*******************************************************************
* addBookToBack
*
* Adds a book entity to the back of a book list
*******************************************************************/
Book* addBookToBack(Book* head, Book* bookToAdd, int* numRecordsUpdated)
{
    if (bookToAdd != NULL)
    {
        if (head != NULL)
        {
            Book* temp = head;
            if (!equals(temp, bookToAdd))
            {
                while (temp->next != NULL)
                {
                    if (equals(temp, bookToAdd))
                    {
                        return head;
                    }
                    temp = temp->next;
                }
                temp->next = bookToAdd;
                *numRecordsUpdated = *numRecordsUpdated + 1;
            }
        }        
        else
        {
            head = bookToAdd;
            *numRecordsUpdated = *numRecordsUpdated + 1;
        }
    }
    return head;
}

/*******************************************************************
* findBooks
*
* Returns a list of books matching the fields provided
*******************************************************************/
Book* findBooks(Book* head, char* bookTitle, char* bookAuthor, int* numBooksFound)
{
    Book* booksFound = NULL;

    if (head != NULL)
    {
        Book* temp = head;
        if (bookTitle != NULL && bookAuthor != NULL)
        {
            while (temp != NULL)
            {
                if (strcmp(temp->title, bookTitle) == 0 && strcmp(temp->author, bookAuthor) == 0)
                {
                    Book* book = createBook(temp->title, temp->author, temp->location);
                    booksFound = addBookToBack(booksFound, book, &(*numBooksFound));
                }
                temp = temp->next;
            }
        }
        else if (bookTitle != NULL && bookAuthor == NULL)
        {
            while (temp != NULL)
            {
                if (strcmp(temp->title, bookTitle) == 0)
                {
                    Book* book = createBook(temp->title, temp->author, temp->location);
                    booksFound = addBookToBack(booksFound, book, &(*numBooksFound));
                }
                temp = temp->next;
            }
        }
        else if (bookTitle == NULL && bookAuthor != NULL)
        {
            while (temp != NULL)
            {
                if (strcmp(temp->author, bookAuthor) == 0)
                {
                    Book* book = createBook(temp->title, temp->author, temp->location);
                    booksFound = addBookToBack(booksFound, book, &(*numBooksFound));
                }
                temp = temp->next;
            }
        }
    }
    return booksFound;
}

/*******************************************************************
* removeBook
*
* Removes a book matching the fields provided from the book list
*******************************************************************/
Book* removeBook(Book* head, char* toRemoveTitle, char* toRemoveAuthor, int* numBooksRemoved)
{
    if (head != NULL)
    {
        Book* temp = head;
        if (strcmp(temp->title, toRemoveTitle) == 0 && strcmp(temp->author, toRemoveAuthor) == 0)
        {
            head = head->next;
            destroyBook(temp);
            *numBooksRemoved = *numBooksRemoved + 1;
        }
        
        if (head != NULL)
        {
            temp = head;
            while (temp->next != NULL)
            {
                if (strcmp(temp->next->title, toRemoveTitle) == 0 
                    && strcmp(temp->next->author, toRemoveAuthor) == 0)
                {
                    Book* toRemove = temp->next;
                    temp->next = temp->next->next;
                    destroyBook(toRemove);
                    *numBooksRemoved = *numBooksRemoved + 1;
                    continue;
                }
                temp = temp->next;
            }
        }
    }
    return head;
}

/*******************************************************************
* appendBookToString
*
* Appends the contents of a book entity to a string
*******************************************************************/
void appendBookToString(Book* head, char* bookString)
{
    while (head != NULL)
    {
        strcat(bookString, head->title);
        strcat(bookString, "\n");
        strcat(bookString, head->author);
        strcat(bookString, "\n");
        strcat(bookString, head->location);
        strcat(bookString, "\n\n");
        head = head->next;
    }
    strcat(bookString, "\0");
}

/*******************************************************************
* destroyBook
*
* Frees dynamic memory allocated to a given book
*******************************************************************/
void destroyBook(Book* bookToFree)
{
    if (bookToFree != NULL)
    {
        if (bookToFree->title != NULL)
        {
            free(bookToFree->title);
        }
        if (bookToFree->author != NULL)
        {
            free(bookToFree->author);
        }
        if (bookToFree->location != NULL)
        {
            free(bookToFree->location);
        }
        free(bookToFree);
        bookToFree = NULL;
    }
}

/*******************************************************************
* destroyBookList
*
* Frees dynamic memory allocated to a given book list
*******************************************************************/
void destroyBookList(Book* bookListToFree)
{
    if (bookListToFree != NULL)
    {
        Book* prev = bookListToFree;
        while (bookListToFree != NULL)
        {
            bookListToFree = bookListToFree->next;
            destroyBook(prev);
            prev = bookListToFree;
        }
    }
}
