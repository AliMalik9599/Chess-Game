
#include "Knight.h"
#include <cmath>
#include <cstdlib>

bool Knight::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const{
  //Knights may move in L shapes.
  if ((std::abs(end.first - start.first) == 2 && std::abs(end.second - start.second) == 1)|| (std::abs(end.second - start.second) == 2 && std::abs(end.first - start.first) == 1)){
    //making sure that the position is valid
    if (end.second < '1' || end.second > '8') {
      return false;
    }
    else if (end.first < 'A' || end.first > 'H') {
      return false;
    }
    //if position is valid and it is an L shape, return true
    else {
      return true;
    }
  }
  else {
    return false;
  }	
}
