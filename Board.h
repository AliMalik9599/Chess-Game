
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"
#include "Terminal.h"


class Board {
  
public:
  // Default constructor
  Board();
  
  ~Board();
  
  Board(const Board &orig);
  // Returns a const pointer to the piece at a prescribed location if it exists,
  // or nullptr if there is nothing there.
  //const Piece* operator() (std::pair<char, char> position) const;
  const Piece* operator() (std::pair<char, char> position) const;
  void remove_piece(std::pair<char,char> location);
  
  // Attempts to add a new piece with the specified designator, at the given location.
  // Returns false if:
  // -- the designator is invalid,
  // -- the specified location is not on the board, or
  // -- if the specified location is occupied
  bool add_piece(std::pair<char, char> position, char piece_designator);
  
  // Displays the board by printing it to stdout
  void display() const;
  
  // Returns true if the board has the right number of kings on it
  bool has_valid_kings() const;

  std::string symbol(const Piece* piece) const;
  
private:
  // The sparse map storing the pieces, keyed off locations
  std::map<std::pair<char, char>, Piece*> occ;
};

// Write the board state to an output stream
std::ostream& operator<< (std::ostream& os, const Board& board);

#endif // BOARD_H
