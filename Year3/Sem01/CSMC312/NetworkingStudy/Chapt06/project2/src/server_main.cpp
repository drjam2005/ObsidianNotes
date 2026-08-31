#include "../include/server.h"

int main() {
	Server server("0.0.0.0", 5050);

	server.run();
}
