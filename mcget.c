// mcget.c
// Created by Larry Williamson and Grant Garrett on 12/8/14.
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
	requestType = htonl(0);
	strcpy(filename, argv[4]);
	clientfd = Open_clientfd(host, port);
/*	end connection setup	*/

/* send parameters to server	*/
	Rio_writen(clientfd, &secretKey, sizeof(unsigned int));
	Rio_writen(clientfd, &requestType, sizeof(unsigned int));
	// check file before sending to server
	FILE *fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}
	Rio_writen(clientfd, filename, 80);
/* end parameter send	*/

/*	pull data from server	*/
	int bytesReceived = 0;
	char buff[BUF_SIZE];
	//memset(buff, '\0', sizeof(buff));
	while((bytesReceived = Rio_readn(clientfd, buff, BUF_SIZE)) > 0)
	{
		fwrite(buff, 1, bytesReceived, fp);
		Fputs(buff, stdout);
	}
/*	end data pull	*/

/* get return status and end connection	*/
	unsigned int status;
	Rio_readn(clientfd, &status, sizeof(unsigned int));
	if(bytesReceived < 0)
	{
		printf("Error.\n");
		return -1;
	}
	if (status == -1)
	{
		printf("Error getting from server.\n");
	}
	fclose(fp);
	Close(clientfd);
	exit(0);
/*	connection closed	*/
}
