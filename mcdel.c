// mcdel.c
// Created by Larry Williamson and Grant Garrett on 12/10/14
#include "csapp.h"
#define BUF_SIZE 256


int main(int argc, char **argv)
{
/*	establish connection	*/
	char *host, filename[80];
	int clientfd, port;
	unsigned int secretKey, requestType;
	if (argc != 5)
	{
		fprintf(stderr, "usage: %s <host> <port> <secret key> <filename>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);
	secretKey = htonl(atoi(argv[3]));
	requestType = htonl(2);
	strcpy(filename, argv[4]);
	clientfd = Open_clientfd(host, port);
/*	end connection setup	*/

/* send parameters to server	*/
	Rio_writen(clientfd, &secretKey, sizeof(unsigned int));
	Rio_writen(clientfd, &requestType, sizeof(unsigned int));
	Rio_writen(clientfd, filename, 80);
/* end parameter send	*/

/*	get return status and end connection	*/
	unsigned int status;
	Rio_readn(clientfd, &status, sizeof(unsigned int));
	/*
	if (status != 0)
	{
		printf("Error\n");
	}
	*/
	Close(clientfd);
	exit(0);
/*	connection closed	*/
}