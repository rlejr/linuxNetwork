#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 256

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[MAXLINE];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
       	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		 perror("ERROR on binding");
		 close(sockfd);
		 exit(1);
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("ERROR on accept");
		close(sockfd);
		exit(1);
	}
	while(1) {
		bzero(buffer, MAXLINE);
		n = read(newsockfd, buffer, MAXLINE-1);
		if (n < 0) {
			perror("ERROR reading from socket");
			break;
		}
		printf("Client: %s\n", buffer);
		printf("Server: ");
		bzero(buffer, MAXLINE);
		fgets(buffer, MAXLINE-1, stdin);
		n = write(newsockfd, buffer, strlen(buffer));
		if (n < 0) {
			perror("ERROR writing to socket");
			break;
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}

