// server.c
// Created by Larry Williamson and Grant Garrett on 12/8/14.
#include "csapp.h"


void echo(int connfd);

int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    unsigned int server_secret_key, secret_key;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    rio_t received; 
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s <port> <secret_key>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);
    server_secret_key = atoi(argv[2]);
    listenfd = Open_listenfd(port);

    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        
        /* Determine the domain name and IP address of the client */
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        
        haddrp = inet_ntoa(clientaddr.sin_addr);
        
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);
        
        unsigned int request_type;
	int bytes_received;
	bytes_received = Rio_readn(connfd, &secret_key, sizeof(unsigned int));
	secret_key = ntohl(secret_key);
	printf("Secret Key = %i\n", secret_key);	
	
	bytes_received = Rio_readn(connfd, &request_type, sizeof(unsigned int));
        int status = 0;
        request_type = ntohl(request_type);
       	printf("rt = %i\n", request_type);
	 if (secret_key != server_secret_key) {
            status = -1;
        }
		if (request_type == 0){
			printf("Request Type = get\n");
			if (status == 0)
			status = get(connfd);
		}
		else if (request_type == 1 && status == 0){
			printf("Request Type = put\n");
			if (status == 0)
			status = put(connfd);
		}
		else if (request_type == 2 && status == 0){
			printf("Request Type = del\n");
			if (status == 0)
			status = del(connfd);
		}
		else if (request_type == 3 && status == 0){
			printf("Request Type = list\n");
			if (status == 0){
				status = list(connfd);
			}
		write(connfd, &status, sizeof(status));
        }
        if (status == 0){
            printf("Operation Status = Success\n");    
        }
        else{
            printf("Operation Status = Error\n");
        }
        printf("--------------------------\n");
        Close(connfd);
    }
    exit(0);
}

int get(connfd){
    int bytes_received, status;
    char buf[MAXLINE];
    memset(buf, '\0', MAXLINE);
    
    status = 0;
    
    bytes_received = Rio_readn(connfd, buf, 80);
    FILE *fp = fopen(buf, "rb");
    if(fp==NULL){status = -1;}
    
    printf("Filename = %s\n", buf);
    
    while (1) {
        unsigned char buf[100240]={0};
        int data = fread(buf,1,sizeof(buf), fp);
        
        if(data > 0)
        {
            Rio_writen(connfd, buf, strlen(buf));
        }
        
        if (data < 100240)
        {
            if (ferror(fp)){
                status = -1;
            }
            break;
        }
    }
    return status;
}


//stubs for stuff
int put(connfd){
	int bytes_received = 0;
        char buff[MAXLINE];
	rio_t rio;

        //memset(buff, '0', sizeof(buff));
       // FILE *fp = fopen(filename, "w");
      //  ssize_t bytes_received; 
	int status = 0;; 

	bytes_received = Rio_readn(connfd, buff, 80);
        FILE *fp = fopen(buff, "w");
        if(fp==NULL){status = -1;}
	printf("Filename = %s\n", buff);
	Rio_readinitb(&rio, connfd);
/*
	if(NULL == fp)
        {
                printf("Error opening file \n");
                return -1;
        }
        //Rio_writen(clientfd, &secretKey, sizeof(unsigned int));

        //Rio_writen(clientfd, &requestType, sizeof(unsigned int));

        //Rio_writen(clientfd, filename, 80); */
	char buff2[MAXLINE];
        while((bytes_received = Rio_readnb(&rio, buff2, 1)) > 0)
        {
	//	printf("Buff: %s\n", buff);
	//	Rio_readn(connfd, buff, MAXLINE);
                fwrite(buff2, 1, 1 , fp);
                Fputs(buff2, stdout);
		fflush(stdout);
        }
	printf("After While");	
	return 0;
}

int del(connfd){
	return 0;
}

int list(connfd){
	return 0;
}

