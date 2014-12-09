// server.c
// Created by Larry Williamson and Grant Garrett on 12/8/14.
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
	unsigned int requestType = 0;
	strcpy(filename, argv[4]);
	clientfd = Open_clientfd(host, port);

	//connected

	int bytesReceived = 0;
	char buff[BUF_SIZE];
	memset(buff, '0', sizeof(buff));
	FILE *fp = fopen(filename, "w");
	if(NULL == fp)
	{
		printf("Error opening file \n");
		return -1;
	}
	Rio_writen(clientfd, &secretKey, sizeof(unsigned int));

	Rio_writen(clientfd, &requestType, sizeof(unsigned int));
	
	printf("%i\n", requestType);

	Rio_writen(clientfd, filename, 80);

	while((bytesReceived = Rio_readn(clientfd, buff, BUF_SIZE)) > 0)
	{
		fwrite(buff, 1, bytesReceived, fp);
	}
	unsigned int status;
	Rio_readn(clientfd, &status, sizeof(unsigned int));
		if (status != 0){
		printf("Error!!!!!\n");
	}
	if(bytesReceived < 0)
	{
		printf("Error \n");
		return -1;
	}

	fclose(fp);
	Close(clientfd);
	exit(0);
}