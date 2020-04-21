/**************************
* Author: Dimitar Dimitrov
* Course: CIS 3210
* Name: Assignment 1
* Last Modified: 11/10/2019
***************************/

#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

/*******************************************************************
* Book
*
* Book entity with title, author, and location fields
*******************************************************************/
typedef struct Book
{
    char* title;
    char* author;
    char* location;
    struct Book* next;
} Book;

/*******************************************************************
* createBook
*
* Creates a book entity using dynamic memory and fields provided
*******************************************************************/
Book* createBook(char* title, char* author, char* location);

/*******************************************************************
* equals
*
* Given two books determines if they have identical fields
*******************************************************************/
int equals(Book* bookOne, Book* bookTwo);

/*******************************************************************
* addBookToBack
*
* Adds a book entity to the back of a book list
*******************************************************************/
Book* addBookToBack(Book* head, Book* bookToAdd, int* numRecordsUpdated);

/*******************************************************************
* findBooks
*
* Returns a list of books matching the fields provided
*******************************************************************/
Book* findBooks(Book* head, char* bookTitle, char* bookAuthor, int* numBooksFound);

/*******************************************************************
* removeBook
*
* Removes a book matching the fields provided from the book list
*******************************************************************/
Book* removeBook(Book* head, char* toRemoveTitle, char* toRemoveAuthor, int* numBooksRemoved);

/*******************************************************************
* appendBookToString
*
* Appends the contents of a book entity to a string
*******************************************************************/
void appendBookToString(Book* head, char* bookString);

/*******************************************************************
* destroyBook
*
* Frees dynamic memory allocated to a given book
*******************************************************************/
void destroyBook(Book* bookToFree);

/*******************************************************************
* destroyBookList
*
* Frees dynamic memory allocated to a given book list
*******************************************************************/
void destroyBookList(Book* bookListToFree);

#endif
