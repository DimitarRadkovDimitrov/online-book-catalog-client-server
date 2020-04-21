USER=dimitar
CC=gcc
CFLAGS= -Wall -std=c99

all: client server 

client: client.o
	$(CC) $(CFLAGS) client.o -o client

client.o: src/client.c
	$(CC) $(CFLAGS) -c src/client.c -Iinclude -o client.o

server: server.o bookCatalogue.o linkedList.o
	$(CC) $(CFLAGS) server.o bookCatalogue.o linkedList.o -lpthread -o server

server.o: src/server.c src/bookCatalogue.c src/linkedList.c
	$(CC) $(CFLAGS) -c src/server.c src/bookCatalogue.c src/linkedList.c -Iinclude

clean:
	rm -f client server *.o

zip:
	zip -r cis3210_ass1_$(USER).zip src include README Makefile cis3210_ass1_design_doc_dimitar.pdf
