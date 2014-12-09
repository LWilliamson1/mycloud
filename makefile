CC      = gcc
OPTIONS = -pthread
OBJECTS = csapp.o

all:	mcget server

mcget:	$(OBJECTS) mcget.c
	$(CC) $^ -o mcget $(OPTIONS)

server:	$(OBJECTS) server.c
	$(CC) $^ -o server $(OPTIONS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o mcget server
