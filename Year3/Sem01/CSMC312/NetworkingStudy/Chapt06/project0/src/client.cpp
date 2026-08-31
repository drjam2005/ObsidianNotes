#include "../include/client.h"
#include "../vendor/include/raylib.h"
#include <cstring>
#include <string>
#include <thread>
#include <signal.h>

Client::Client(int port) {
	_setup_connections(port);

	for(int y = 0; y < 3; ++y) {
		for(int x = 0; x < 3; ++x) {
			table[y][x] = ' ';
		}
	}
}

void Client::_setup_connections(int port) {
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

		connection = connect(sockfd, p->ai_addr, p->ai_addrlen);
		if(connection == -1) {
			fprintf(stderr, "error trying to connect socket\n");
			continue;
		}

		break;
	}

	PACKET packet;
	int bytes = recv(sockfd, &packet, sizeof packet, 0);
	if(!(bytes == sizeof packet)){
		return;
	}

	if(strcmp(packet.buffer, "You are Player X") == 0){
		playerType = 1;
	}else if(strcmp(packet.buffer, "You are Player O") == 0){
		playerType = 2;
	}

	std::thread async(&Client::_handle_connections, this);
	async.detach();
}

void Client::_handle_connections() {
	while(true) {
		PACKET tablePacket;
		int bytes = recv(sockfd, &tablePacket, sizeof tablePacket, 0);
		if(bytes != 0 && bytes != sizeof tablePacket)
			CloseWindow();

		memcpy(table, tablePacket.table, 3*3*(sizeof(char)));
		currentTurn = tablePacket.turn;
	}
}

bool Client::run() {
	if(connection == -1)
		return false;

	if(playerType == 1){
		DrawText("You are Player X", 20, 20, 40, WHITE);
	} else if(playerType == 2){
		DrawText("You are Player O", 20, 20, 40, WHITE);
	}

	if(currentTurn == playerType) {
		DrawText("Your Turn", 20, 60, 40, WHITE);
		// move
		for(int y = 0; y < 3; ++y) {
			for(int x = 0; x < 3; ++x) {
				Rectangle rec = {100.0f + (50*x), 100.0f + (50*y), 50, 50};

				if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
					if(CheckCollisionPointRec(GetMousePosition(), rec)) {
						PACKET packet;
						packet.type = PACKET_USER_MOVE;
						packet.move_loc = (y*3) + x;

						send(sockfd, &packet, sizeof packet, 0);
					}
				}
			}
		}
	}
	for(int y = 0; y < 3; ++y) {
		for(int x = 0; x < 3; ++x) {
			DrawRectangle(100 + (50*x), 100 + (50*y), 50, 50, RED);
			DrawRectangleLinesEx({100.0f + (50*x), 100.0f + (50*y), 50, 50}, 5, BLACK);
			DrawText(TextFormat("%c", table[y][x]), 120 + (50*x), 120 + (50*y), 20, BLACK);
		}
	}

	return true;
}


int main(int argc, char** argv) {
	if(argc != 2){
		fprintf(stderr, "usage: ./client <PORT_NUMBER>\n");
		return 1;
	}

	Client client(std::stoi(argv[1]));

	SetTraceLogLevel(LOG_NONE);
	InitWindow(800, 600, "hehe");

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		if(!client.run())
			CloseWindow();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
