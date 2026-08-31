#include "../include/server.h"
#include <cstring>
#include <string>
#include <signal.h>

Server::Server(int port) {
	for(int y = 0; y < 3; ++y) {
		for(int x = 0; x < 3; ++x) {
			table[y][x] = ' ';
		}
	}

	_setup_connections(port);
}

void Server::_setup_connections(int port) {
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	struct sigaction sa;
	int yes = 1;

	if(getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servInfo) != 0) {
		fprintf(stderr, "error connecting to 0.0.0.0 on port %d\n", port);
		return;
	}

	for(p = servInfo; p != NULL; p = p->ai_next){
		// attempt creation fof a socket
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd == -1) {
			fprintf(stderr, "error trying to create socket\n");
			continue;
		}

		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		char buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipv4->sin_addr, buffer, sizeof buffer);

		printf("currently found address: %s\n", buffer);

		int val = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if(val == -1) {
			fprintf(stderr, "error trying to bind socket\n");
			continue;
		}

		break;
	}

	listen(sockfd, 2);

	while (!playerX || !playerO) {
		struct sockaddr_storage client_addr;
		socklen_t client_addr_len = sizeof client_addr;
		int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
		if(!playerX) {
			playerX = clientfd;
			PACKET packet;
			packet.type = PACKET_MESSAGE;
			strcpy(packet.buffer, "You are Player X");
			send(playerX, &packet, sizeof packet, 0);
		}
		else {
			playerO = clientfd;
			PACKET packet;
			packet.type = PACKET_MESSAGE;
			strcpy(packet.buffer, "You are Player O");
			send(playerO, &packet, sizeof packet, 0);

			currentTurn = 1;
			PACKET tablePacket;

			tablePacket.type = PACKET_TABLE;
			tablePacket.turn = 1;
			memcpy(tablePacket.table, table, 3*3*(sizeof(char)));

			send(playerX, &tablePacket, sizeof tablePacket, 0);
			send(playerO, &tablePacket, sizeof tablePacket, 0);
		}

		char ip_addr[INET_ADDRSTRLEN] = {0};
		inet_ntop(AF_INET,  &((struct sockaddr_in*)&client_addr)->sin_addr, ip_addr, sizeof ip_addr);

		printf("found clientfd: %d, with IP: %s\n", clientfd, ip_addr);
	}
}

void Server::run() {
	_handle_connections();
}

void Server::_print_table() {
	for(int y = 0; y < 3; ++y) {
		for(int x = 0; x < 3; ++x) {
			printf("%c ", table[y][x]);
		}
		printf("\n");
	}
}

void Server::_handle_connections() {
	while(true) {
		if(!(playerX && playerO))
			continue;

		if (currentTurn == 1) {
			PACKET packet;
			int bytes = recv(playerX, &packet, sizeof packet, 0);
			
			if(bytes != 0 && bytes != sizeof packet) {
				fprintf(stderr, "playerX recv bytes not same as packet\n");
				return;
			}

			if(packet.type == PACKET_USER_MOVE) {
				int x = packet.move_loc % 3;
				int y = packet.move_loc / 3;

				if(table[y][x] != ' ')
					continue;

				table[y][x] = 'X';
			}

			PACKET tablePacket;
			tablePacket.type = PACKET_TABLE;
			tablePacket.turn = 2;
			memcpy(tablePacket.table, table, 3*3*(sizeof(char)));

			send(playerX, &tablePacket, sizeof tablePacket, 0);
			send(playerO, &tablePacket, sizeof tablePacket, 0);

			currentTurn = 2;

		} else if (currentTurn == 2) {
			PACKET packet;
			int bytes = recv(playerO, &packet, sizeof packet, 0);

			if(bytes != 0 && bytes != sizeof packet) {
				fprintf(stderr, "playerO recv bytes not same as packet\n");
				return;
			}

			if(packet.type == PACKET_USER_MOVE) {
				int x = packet.move_loc % 3;
				int y = packet.move_loc / 3;

				if(table[y][x] != ' ')
					continue;

				table[y][x] = '0';
			}
			
			PACKET tablePacket;
			tablePacket.type = PACKET_TABLE;
			tablePacket.turn = 1;
			memcpy(tablePacket.table, table, 3*3*(sizeof(char)));

			send(playerX, &tablePacket, sizeof tablePacket, 0);
			send(playerO, &tablePacket, sizeof tablePacket, 0);

			currentTurn = 1;
		}
	}
}

int main(int argc, char** argv) {
	if(argc != 2){
		fprintf(stderr, "usage: ./server <PORT_NUMBER>\n");
		return 1;
	}

	Server server(std::stoi(argv[1]));
	server.run();
}
