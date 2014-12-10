// mclist.c
// Created by Larry Williamson and Grant Garrett 12/10/14
#include "csapp.h"
#define BUF_SIZE 256


int main(int argc, char **argv)
{
	int clientfd, port;
	char *host, buf[MAXLINE];
	int bytesReceived = 0;
	rio_t rio;
	if (argc != 4) {
	fprintf(stderr, "usage: %s <host> <port> <secret key>\n", argv[0]);
	exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);
	unsigned int secretKey = htonl(atoi(argv[3]));
	unsigned int requestType = htonl(3);
	clientfd = Open_clientfd(host, port);

	//connected

	int bytesSent = 0;
	char buff[BUF_SIZE];
	//memset(buff, '0', sizeof(buff));

	Rio_writen(clientfd, &secretKey, sizeof(unsigned int));
	Rio_writen(clientfd, &requestType, sizeof(unsigned int));

	while((bytesReceived = Rio_readn(clientfd, buff, BUF_SIZE)) > 0)
	{
		printf("%s", buff);
	}
}