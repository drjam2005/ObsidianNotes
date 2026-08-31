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

class Server {
private:
	int sockfd;
	int currentTurn = 0;
	int playerX = 0;
	int playerO = 0;
	char s[INET6_ADDRSTRLEN];
	struct addrinfo hints, *servInfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;

	char table[3][3];
public:
	Server(int port);
	void run();
private:
	void _setup_connections(int port);
	void _handle_connections();
	void _manage_client(int client_fd);

	void _print_table();

};

#endif
