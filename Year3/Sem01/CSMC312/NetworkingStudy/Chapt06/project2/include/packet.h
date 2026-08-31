#pragma once
#ifndef PACKET_H
#define PACKET_H

enum PacketType {
	PACKET_UserJoin    = 0,
	PACKET_UserMessage,
	PACKET_UserLeave,

	PACKET_Count
};

struct Packet {
	PacketType type;

	int userID;			// UserJoin, UserLeave
	char buffer[256];	// UserMessage
};

#endif // PACKET_H
