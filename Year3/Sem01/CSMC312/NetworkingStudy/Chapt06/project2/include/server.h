#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "./socket.h"
#include <vector>

struct Connection {
	int id;
	Socket client_fd;
};

class Server {
private:
	TcpServer tcpServer;

	const char* hostname;
	int port;
	
	std::vector<Connection> clients;
public:
	Server(const char* host, int port);
	
	void handleJoin();

	void run();
};

#endif // SERVER_H
