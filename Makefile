CC = gcc
CFLAGS = -pedantic -Wall -Wextra -g

.PHONY: all clean test lib

all: UDP_server

UDP_server: UDP_server.o 
	$(CC) $(CFLAGS) UDP_server.o -o UDP_server

UDP_server.o: UDP_server.c
	$(CC) $(CFLAGS) -c UDP_server.c -o UDP_server.o


test: linkedlist_test

linkedlist_test: linkedlist_test.o linkedlist.o linkedlist.h
	$(CC) $(CFLAGS) linkedlist_test.o linkedlist.o -o linkedlist_test

linkedlist_test.o: linkedlist_test.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist_test.c -o linkedlist_test.o

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c -o linkedlist.o

libqueue.a: linkedlist.o linkedlist.h
	ar rcs libqueue.a linkedlist.o

clean:
	rm -rf UDP_server UDP_server.o linkedlist_test linkedlist_test.o linkedlist.o

lib: libqueue.a