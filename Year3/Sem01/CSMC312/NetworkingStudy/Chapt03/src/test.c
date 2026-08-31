#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
	int status;
	struct addrinfo hints;
	struct addrinfo *servInfo; // will point to results

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((status = getaddrinfo(NULL, "6767", &hints, &servInfo)) != 0 ) {
		fprintf(stderr, "gai error: %s\n", gai_strerror(status));
		return 1;
	}

	for(struct addrinfo* p = servInfo; p != NULL; p = p->ai_next) {
		char address[INET6_ADDRSTRLEN];

		if(p->ai_family == AF_INET){
			struct sockaddr_in *ipv4 = (struct sockaddr_in*)p->ai_addr;
			inet_ntop(AF_INET, &ipv4->sin_addr, address, INET_ADDRSTRLEN);

		}else if(p->ai_family == AF_INET6){
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*)p->ai_addr;
			inet_ntop(AF_INET6, &ipv6->sin6_addr, address, INET6_ADDRSTRLEN);

		}

		printf("address: %s\n", address);
	}

	// code...

	freeaddrinfo(servInfo);
}
