#pragma once 
#ifndef CLIENT_H
#define CLIENT_H

#include "socket.h"

class Client {
private:
	TcpClient connection;
public:

	void run();
};

#endif // CLIENT_H
