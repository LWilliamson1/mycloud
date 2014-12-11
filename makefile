CC      = gcc
OPTIONS = -pthread
OBJECTS = csapp.o

all:	mcput mcget mclist mcdel mycloud_server

mcput:	$(OBJECTS) mcput.c
	$(CC) $^ -o mcput $(OPTIONS)

mcget:	$(OBJECTS) mcget.c
	$(CC) $^ -o mcget $(OPTIONS)

mcdel:	$(OBJECTS) mcdel.c
	$(CC) $^ -o mcdel $(OPTIONS)

mclist:	$(OBJECTS) mclist.c
	$(CC) $^ -o mclist $(OPTIONS)

mycloud_server:	$(OBJECTS) mycloud_server.c
		$(CC) $^ -o mycloud_server $(OPTIONS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o mcput mcget mclist mcdel mycloud_server
