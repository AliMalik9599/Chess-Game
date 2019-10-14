#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include "Piece.h"
#include "Board.h"

class Chess {

public:
  // This default constructor initializes a board with the standard
  // piece positions, and sets the state to white's turn
  Chess();
  Chess(const Board &insboard, bool white);
  ~Chess();
  
  // Returns a constant reference to the board 
  const Board& get_board() const { return board; }
  
  // Returns true if it is white's turn
  bool turn_white() const { return is_white_turn; }
  
  bool within_bounds(std::pair<char, char> position) const;

  bool in_the_way(std::pair<char,char> start, std::pair<char,char> end) const;
  
  
  
  std::string pseudo_move(std::pair<char, char> start, std::pair<char, char> end) const;
    
  // Attemps to make a move. If successful, the move is made and
  // the turn is switched white <-> black
  bool make_move(std::pair<char, char> start, std::pair<char, char> end);
  // Returns true if the designated player is in check
  bool in_check(bool white) const;

  void change_turn(char turn);
  
  bool mate_helper_white() const;
  bool mate_helper_black() const;
  // Returns true if the designated player is in mate
	bool in_mate(bool white) const;
  
  // Returns true if the designated player is in mate
  bool in_stalemate(bool white) const;
  void clear_board();
  bool add_load(std::pair<char,char> loc, char designator);

private:
  // The board
  Board board;
  
  // Is it white's turn?
  bool is_white_turn;
  
};

// Writes the board out to a stream
std::ostream& operator<< (std::ostream& os, const Chess& chess);

// Reads the board in from a stream
std::istream& operator>> (std::istream& is, Chess& chess);


#endif // CHESS_H
