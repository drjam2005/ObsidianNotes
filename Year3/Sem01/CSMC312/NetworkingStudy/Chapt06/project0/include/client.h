#pragma once
#include <type_traits>
#ifndef SERVER_H
#define SERVER_H

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

#include <thread>
#include <mutex>
#include <unordered_map>

#include "packets.h"

class Client {
private:
	int sockfd;
	int connection;
	int currentTurn = 0;

	int playerType;
	char s[INET6_ADDRSTRLEN];
	struct addrinfo hints, *servInfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;

	char table[3][3];
public:
	Client(int port);

	bool run();
private:
	void _setup_connections(int port);

	void _handle_connections();
};

#endif
