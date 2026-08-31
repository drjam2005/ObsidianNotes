// server.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define ADDRESS "0.0.0.0"
#define PORT "42069"


int main() {
	// define hints
	struct addrinfo hints, *servInfo, *p;
	int sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(ADDRESS, PORT, &hints, &servInfo) != 0) {
		fprintf(stderr, "error connecting to "ADDRESS" on port "PORT"\n");
		return 0;
	}

	for(p = servInfo; p != NULL; p = p->ai_next){
		// attempt creation fof a socket
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd == -1) {
			fprintf(stderr, "error trying to create socket\n");
			continue;
		}

		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		char buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipv4->sin_addr, buffer, sizeof buffer);

		printf("currently found address: %s\n", buffer);

		int val = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if(val == -1) {
			fprintf(stderr, "error trying to bind socket\n");
			continue;
		}

		break;
	}

	printf("created socket on %d\n", sockfd);
	printf("now listening on "ADDRESS" on port "PORT" through socket %d\n", sockfd);

	listen(sockfd, 5);

	while (1) {
		struct sockaddr_storage client_addr;
		socklen_t client_addr_len = sizeof client_addr;
		int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);

		char buffer[256] = {0};

		inet_ntop(AF_INET,  &((struct sockaddr_in*)&client_addr)->sin_addr, buffer, sizeof buffer);

		printf("found clientfd: %d, with IP: %s\n", clientfd, buffer);

		// send message
		send(clientfd, "hi po...", sizeof("hi po..."), 0);
	}
}
