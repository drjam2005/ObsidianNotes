#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "../network/socket.h"
#include "../game/game.h"


class Server {
private:
	TcpServer server;

	Socket playerX;
	Socket playerO;

	Game game;
public:
	void run();
};

#endif
