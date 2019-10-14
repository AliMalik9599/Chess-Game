#include"Rook.h"

bool Rook::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //Rooks can move on straight lines. so straight lines have one of the below conditions true.
  //we worry about in bounds check in psuedo_make_move()
  if((start.first == end.first) || (start.second == end.second)){
    return true;
  } else {
    return false;
  }
		
}
