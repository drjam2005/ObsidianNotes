#include "../../include/game/board.h"

Board::Board() {
	for(int y = 0; y < 3; ++y){
		for(int x = 0; x < 3; ++x){
			cells[y][x] = ' ';
		}
	}
}

bool Board::make_move(int position, char symbol) {
	int x = position % 3;
	int y = int(position / 3);

	if(cells[y][x] != ' ')
		return false;

	cells[y][x] = symbol;
	return true;
}

bool Board::is_full(){
	for(int y = 0; y < 3; ++y){
		for(int x = 0; x < 3; ++x){
			if(cells[y][x] != ' ')
				return true;
		}
	}

	return false;
}

bool Board::has_winner(){
	// winner parsing ehh
	return false;
}

const char* Board::data() const {
	return (char*)this->cells;
}
