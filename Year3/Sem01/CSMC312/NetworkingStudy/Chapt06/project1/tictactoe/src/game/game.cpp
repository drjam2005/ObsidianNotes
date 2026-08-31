#include "../../include/game/game.h"

Game::Game() {
	// ...
}

bool Game::make_move(Player player, int position) {
	return this->board.make_move(position, (char)player);
}
