#include "King.h"
#include <cmath>
#include <cstdlib>

bool King::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //kings can move one forward or back, or one right one left
  if(std::abs(end.second-start.second)==1||std::abs(end.second-start.second)==0){
    if(std::abs(end.first-start.first)==1||std::abs(end.first-start.first)==0){
      return true;
    }
  }
  //no other movements are valid.
  else{
    return false;
  }
  return false;
}


