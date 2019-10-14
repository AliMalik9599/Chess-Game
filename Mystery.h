#ifndef MYSTERY_H
#define MYSTERY_H

#include "Piece.h"
#include <cmath>
#include <cstdlib>
class Mystery : public Piece {

public:
  bool legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
    if(std::abs(end.first - start.first) == std::abs(end.second - start.second)){return true;}
    // (void) start;//meaningless statement helps avoid unused parameter warning for now
    //(void) end;  //meaningless statement helps avoid unused parameter warning for now
    return false;
  }
  
  char to_ascii() const {
    return is_white() ? 'M' : 'm';
  }
  
private:
  Mystery(bool is_white) : Piece(is_white) {}
  
  friend Piece* create_piece(char piece_designator);
};

#endif // MYSTERY_H
