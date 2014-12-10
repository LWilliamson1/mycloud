// mclist.c
// Created by Larry Williamson and Grant Garrett 12/10/14
#include "csapp.h"
#define BUF_SIZE 256


int main(int argc, char **argv)
{
/*	establish connection	*/
	char *host;
	int clientfd, port;
	unsigned int secretKey, requestType;
	if (argc != 4)
	{
		fprintf(stderr, "usage: %s <host> <port> <secret key>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);
	secretKey = htonl(atoi(argv[3]));
	requestType = htonl(1);
	clientfd = Open_clientfd(host, port);
/*	end connection setup	*/

/* send parameters to server	*/
	Rio_writen(clientfd, &secretKey, sizeof(unsigned int));
	Rio_writen(clientfd, &requestType, sizeof(unsigned int));
/* end parameter send	*/

/* receive file names from server	*/
	int bytesReceived = 0;
	char buff[BUF_SIZE];
	while((bytesReceived = Rio_readn(clientfd, buff, BUF_SIZE)) > 0)
	{
		printf("%s", buff);
	}
/*	end receiving	*/

/* get return status and end connection	*/
	unsigned int status;
	Rio_readn(clientfd, &status, sizeof(unsigned int));
	if(bytesReceived < 0)
	{
		printf("Error.\n");
		return -1;
	}
	if (status == 0)
	{
		printf("Error listing from server.\n");
	}
	Close(clientfd);
	exit(0);
/*	connection closed	*/
}