// server.c
// Created by Larry Williamson and Grant Garrett on 12/8/14.
#include "csapp.h"

char files[20][80]={""};
unsigned int secret_key, server_secret_key;
void echo(int connfd);

int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    //unsigned int server_secret_key, secret_key;
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
        
 //       printf("server connected to %s (%s)\n", hp->h_name, haddrp);
        
        unsigned int request_type;
	int bytes_received;
	bytes_received = Rio_readn(connfd, &secret_key, sizeof(unsigned int));
	secret_key = ntohl(secret_key);
	printf("Secret Key = %i\n", secret_key);	
	
	bytes_received = Rio_readn(connfd, &request_type, sizeof(unsigned int));
        int status = 0;
        request_type = ntohl(request_type);
	secret_key != server_secret_key;

/*	if (secret_key != server_secret_key) {
	    status = -1;
        }
*/		if (request_type == 0){
			printf("Request Type = get\n");
			status = get(connfd);
		}
		else if (request_type == 1){
			printf("Request Type = put\n");
			status = put(connfd);
		}
		else if (request_type == 2){
			printf("Request Type = del\n");
			status = del(connfd);
		}
		else if (request_type == 3){
			printf("Request Type = list\n");
			status = list(connfd);
		}
	//	unsigned int stat1 = htonl(status);
	//	printf("stat = %i", status);
		write(connfd, &status, sizeof(status));
        
        if (status == 0){
            printf("Operation Status = success\n");    
        }
        else{
            printf("Operation Status = error\n");
        }
        printf("--------------------------\n");
        Close(connfd);
    }
    exit(0);
}

int check_key(){
	if(secret_key == server_secret_key)
	{
		return 0;
	}
	return -1;
}

int get(connfd){
    int bytes_received, status;
    char buf[MAXLINE];
    memset(buf, '\0', MAXLINE);
    
    status = 0;
    
    bytes_received = Rio_readn(connfd, buf, 80);
    printf("Filename = %s\n", buf); 
    FILE *fp = fopen(buf, "rb");
    if(fp==NULL){return -1;}
    if(check_key() == -1) {return -1;} 
    
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
                return -1;
            }
            break;
        }
    }
    return 0;
}


//stubs for stuff
int put(connfd){
	int bytes_received = 0;
        char buff[MAXLINE];
	rio_t rio;

        //memset(buff, '0', sizeof(buff));
       // FILE *fp = fopen(filename, "w");
      //  ssize_t bytes_received; 
	int status = 0; 
	bytes_received = Rio_readn(connfd, buff, 80);
	printf("Filename = %s\n", buff);

	if(check_key() == -1) {return -1;}

        FILE *fp = fopen(buff, "wb");
        if(fp==NULL){status = -1;}


        int i = 0;
	
        while(i < 20)
        {
		//printf("strlen = %i\n",(int)strlen(files[i]));
                if(strcmp(files[i], buff)==0){
			break;
		}
		if(strlen(files[i]) == 0)
                {
			strcpy(files[i], buff);
			//printf("file: %s\n",files[i]);
			break;
                }
		i++;
        }

	int size, bytes;
	bytes = Rio_readn(connfd, &size, sizeof(unsigned int));
	size = ntohl(size);
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
	i = 0;
       // while((bytes_received = Rio_readnb(&rio, buff2, 1)) > 0)
        while(i < size)
	{

//		bytes_received = Rio_readn(&rio, buff2, 1);
		bytes_received = Rio_readn(connfd, buff2, 1);
		printf("Buff: %s\n", buff);
	//	Rio_readn(connfd, buff, MAXLINE);
                fwrite(buff2, 1, 1 , fp);
		i++;
	}
	
	fclose(fp);
	return 0;
}

int del(connfd){
	int status =0;
	char buff[MAXLINE];
	int bytes_received = Rio_readn(connfd, buff, 80);
	
	printf("Filename = %s\n", buff);
	
	if(check_key() == -1) {return -1;}

	if(remove(buff) != 0){
		return -1;
	}

	int i = 0;
	while(i < 20){
		if(strcmp(files[i], buff) == 0){
			strcpy(files[i], "");
			break;
		}
		i++;
	}
	int index = i;
	while(index+1<20){
		strcpy(files[index], files[index+1]);
		index++;
	}
	return 0;
}

int list(connfd){
	int i, status;
	char buff[MAXLINE];
	printf("Filename = NONE\n");
	if(check_key() == -1) {return -1;}
	i = 0;
	if(strlen(files[0])==0){
		return 0;
	}
	
	while(strlen(files[i]) != 0 && i < 20)
	{
		//printf("files: %s\n", files[i]);
		Rio_writen(connfd, files[i], strlen(files[i]));
		Rio_writen(connfd, "\n", strlen("\n"));
		i++;
	}

	return 0;
}



