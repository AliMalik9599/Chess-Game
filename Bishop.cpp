
#include<iostream>
#include <cmath>
#include <cstdlib>
#include "Bishop.h"


bool Bishop::legal_move_shape(std::pair<char,char> start, std::pair<char,char> end) const{
  //Bishops can only move on diagonal, so the differences must equal one another.
  if(std::abs(start.first - end.first) == std::abs(start.second - end.second)){
  	return true;
  } else{
  	return false;
  }
}

