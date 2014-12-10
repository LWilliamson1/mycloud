CC      = gcc
OPTIONS = -pthread
OBJECTS = csapp.o

all:	mcput mcget server

mcput:	$(OBJECTS) mcput.c
	$(CC) $^ -o mcput $(OPTIONS)

mcget:	$(OBJECTS) mcget.c
	$(CC) $^ -o mcget $(OPTIONS)

server:	$(OBJECTS) server.c
	$(CC) $^ -o server $(OPTIONS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o mcput mcget server
