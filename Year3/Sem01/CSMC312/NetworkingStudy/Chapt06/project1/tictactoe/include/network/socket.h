#pragma once
#ifndef SOCKET_H
#define SOCKET_H
#include <stddef.h>

class Socket {
private:
	int sockfd;

public:
	Socket();
	explicit Socket(int fd);

	~Socket();

	bool send(const void* data, size_t size);
	bool recv(void* data, size_t size);

	void close();
};

class TcpServer {
public:
	void listen(int port);
	Socket accept();
};

class TcpClient {
public:
	void connect(const char* host, int port);
};

#endif
