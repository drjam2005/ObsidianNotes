#pragma once
#ifndef GAME_H

#include "./board.h"

enum class Player {
	PLAYER_X,
	PLAYER_O
};

class Game {
private:
	Board board;
	Player turn;
public:
	Game();

	bool make_move(Player player, int position);
	Player current_turn();
	const Board& getBoard();
};


#endif 
