// mcput.c
// Created by Larry Williamson and Grant Garrett on 12/9/14
#include "csapp.h"
#define BUF_SIZE 256


int main(int argc, char **argv)
{
	int clientfd, port;
	char *host, buf[MAXLINE], filename[80];
	rio_t rio;
	if (argc != 5) {
	fprintf(stderr, "usage: %s <host> <port> <secret key> <filename>\n", argv[0]);
	exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);
	unsigned int secretKey = htonl(atoi(argv[3]));
	unsigned int requestType = htonl(1);
	strcpy(filename, argv[4]);
	clientfd = Open_clientfd(host, port);

	//connected

	int bytesSent = 0;
	char buff[BUF_SIZE];
	//memset(buff, '0', sizeof(buff));

	Rio_writen(clientfd, &secretKey, sizeof(unsigned int));

	Rio_writen(clientfd, &requestType, sizeof(unsigned int));

	Rio_writen(clientfd, filename, 80);
	
	while (fgets(buff, BUF_SIZE, stdin) != NULL)
	{
		Rio_writen(clientfd, buff, strlen(buff));
	}
	
	
	unsigned int status;
	Rio_readn(clientfd, &status, sizeof(unsigned int));
	if (status != 0){
		printf("Error\n");
	}
	Close(clientfd);
	exit(0);
}
