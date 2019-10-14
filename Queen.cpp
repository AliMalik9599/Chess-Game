#include"Queen.h"
#include <cmath>
#include <cstdlib>

bool Queen::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //making sure that start and end positions are the same
  if (start.first == end.first && start.second == end.second) {
    return false;
  }
  
  //making sure start and end positions are on the board,
  //and queens can move in straight lines
  if (start.first == end.first || start.second == end.second) {
    if (end.second < '1' || end.second > '8') {
      return false;
    }
    else if (end.first < 'A' || end.first > 'H') {
      return false;
    }
    else {
      return true;
    }
  }
  //queens may move on diagonals.
  else {
    if (std::abs(end.first - start.first) == std::abs(end.second - start.second)) {
      return true;
    }
    else {
      return false;
    }
  }
}
