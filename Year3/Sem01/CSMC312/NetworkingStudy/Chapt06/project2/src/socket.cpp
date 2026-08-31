#include "../include/socket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <string>

Socket::Socket() : sockfd(-1) { }
Socket::Socket(int fd) : sockfd(fd) { }

Socket::~Socket() {
	if(this->sockfd != -1)
		close(sockfd);
}

void Socket::SetFD(int fd) {
	if(sockfd != -1)
		close(sockfd);

	this->sockfd = fd;
}


bool Socket::Send(const char* data, size_t size) {
    size_t sentTotal = 0;

    while (sentTotal < size) {
        ssize_t sent = send(
            sockfd,
            data + sentTotal,
            size - sentTotal,
            0
        );

        if (sent == -1)
            return false;

        sentTotal += sent;
    }

    return true;
}

size_t Socket::Recv(char* data, size_t size) {
    size_t receivedTotal = 0;

    while (receivedTotal < size) {
        ssize_t received = recv(
            sockfd,
            data + receivedTotal,
            size - receivedTotal,
            0
        );

        if (received == -1)
            return false;

        if (received == 0)
            return false;

        receivedTotal += received;
    }

    return true;
}

int Socket::get_fd() const {
    return sockfd;
}

bool TcpServer::Listen(const char* hostname, int port) {
	struct addrinfo hints, *servInfo, *p;
	socklen_t sin_size;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int yes = 1;

	if(getaddrinfo(hostname, std::to_string(port).c_str(), &hints, &servInfo) != 0) {
		fprintf(stderr, "error connecting to 0.0.0.0 on port %d\n", port);
		return false;
	}

	int sockfd = -1;
	for(p = servInfo; p != NULL; p = p->ai_next){
		// attempt creation fof a socket
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd == -1) {
			fprintf(stderr, "error trying to create socket\n");
			continue;
		}

		setsockopt(
			sockfd,
			SOL_SOCKET,
			SO_REUSEADDR,
			&yes,
			sizeof yes
		);

		int val = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if(val == -1) {
			fprintf(stderr, "error trying to bind socket\n");
			close(sockfd);
			continue;
		}

		break;
	}

	freeaddrinfo(servInfo);

	if(sockfd == -1)
		return false;

	if (listen(sockfd, 5) == -1) {
		perror("listen");
		close(sockfd);
		return false;
	}

	this->serverSocket.SetFD(sockfd);

	return true;
}

Socket TcpServer::Accept() {
    sockaddr_storage client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    int clientfd = accept(
        serverSocket.get_fd(),
        reinterpret_cast<sockaddr*>(&client_addr),
        &client_addr_len
    );

    if (clientfd == -1) {
        perror("accept");
        return Socket();
    }

    return Socket(clientfd);
}

bool TcpClient::Connect(const char* host, int port) {
	struct addrinfo hints, *servInfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int yes = 1;

	if(getaddrinfo(host, std::to_string(port).c_str(), &hints, &servInfo) != 0) {
		fprintf(stderr, "error connecting to 0.0.0.0 on port %d\n", port);
		return false;
	}

	for(p = servInfo; p != NULL; p = p->ai_next){
		// attempt creation fof a socket
		int sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd == -1) {
			fprintf(stderr, "error trying to create socket\n");
			continue;
		}

		int connection = connect(sockfd, p->ai_addr, p->ai_addrlen);
		if(connection == -1) {
			fprintf(stderr, "error trying to connect socket\n");
			close(sockfd);
			continue;
		}

		this->clientSocket.SetFD(sockfd);
		freeaddrinfo(servInfo);
		return true;
	}

	return false;
}
