#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {

public:
	bool legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const;
	
	
	char to_ascii() const {
		return is_white() ? 'N' : 'n';
	}

private:
		Knight(bool is_white) : Piece(is_white) {}

	friend Piece* create_piece(char piece_designator);
};

#endif // KNIGHT_H
