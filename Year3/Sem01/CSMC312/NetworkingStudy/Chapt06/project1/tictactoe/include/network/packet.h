#pragma once
#ifndef PACKET_H
#define PACKET_H

enum class PacketType {
	PACKET_PlayerAssignment,
	PACKET_BoardUpdate,
	PACKET_PlayerMove,
	PACKET_GameMessage
};

struct Packet {
	PacketType type;

	int move;
	int turn;
	
	char board[3][3];
	char message[256];
};

#endif
