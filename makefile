CC      = gcc
OPTIONS = -pthread
OBJECTS = csapp.o

all:	mcput mcget mclist mcdel server

mcput:	$(OBJECTS) mcput.c
	$(CC) $^ -o mcput $(OPTIONS)

mcget:	$(OBJECTS) mcget.c
	$(CC) $^ -o mcget $(OPTIONS)

mcdel:	$(OBJECTS) mcdel.c
	$(CC) $^ -o mcdel $(OPTIONS)

mclist:	$(OBJECTS) mclist.c
	$(CC) $^ -o mclist $(OPTIONS)

server:	$(OBJECTS) server.c
	$(CC) $^ -o server $(OPTIONS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o mcput mcget mclist mcdel server
