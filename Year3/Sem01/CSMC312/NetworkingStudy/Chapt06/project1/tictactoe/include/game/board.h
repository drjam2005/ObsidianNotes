#pragma once
#ifndef BOARD_H
#define BOARD_H

class Board {
private:
	char cells[3][3];
public:
	Board();

	bool make_move(int position, char symbol);
	bool is_full();
	bool has_winner();

	const char* data() const;
};

#endif 

