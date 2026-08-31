#pragma once
#ifndef _PACKETS_H
#define _PACKETS_H

#include <cstddef>

enum PACKET_TYPE {
	PACKET_USER_MOVE, 	// for server
	PACKET_MESSAGE,
	PACKET_TABLE,
};

struct PACKET {
	PACKET_TYPE type;
	char buffer[256];
	char table[3][3];
	int move_loc;
	int turn;
};

#endif
