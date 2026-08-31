#pragma once
#ifndef SOCKET_H
#define SOCKET_H

#include <stddef.h>

#include "packet.h"

class Socket {
private:
    int sockfd;

public:
    Socket();
    Socket(int fd);
    ~Socket();

    void SetFD(int fd);

    bool Send(const char* data, size_t size);
    size_t Recv(char* data, size_t size);

    int get_fd() const;
};

class TcpServer {
	Socket serverSocket;
public:
	bool Listen(const char* hostname, int port);
	Socket Accept();
};

class TcpClient {
private:
	Socket clientSocket;
public:
	bool Connect(const char* host, int port);
};

#endif
