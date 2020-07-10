CC=gcc
FLAGS=-Wall -g
LIBFLAGS=-Wall -g -shared

all: IPC

IPC: IPC.o	
	$(CC) $(FLAGS) -o IPC IPC.o


IPC.o: IPC.c
	$(CC) $(FLAGS) -c IPC.c





.PHONY: clean all

clean:
	rm -f IPC IPC.o