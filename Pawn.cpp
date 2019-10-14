#include <cmath>
#include <cstdlib>
#include"Pawn.h"

bool Pawn::legal_move_shape(std::pair<char,char> start, std::pair<char,char> end) const{
  //checking to see that the pawn doesn't change columns
  if(start.first == end.first){
    //checking pawn color
    if(this->to_ascii() == 'P'){
      //pawns may move one space forward (white side end > start)
      if(end.second - start.second == 1){return true;}
      //pawns may move two spaces on their first turn, or one space if desired
      else if((start.second == '2') && ((end.second - start.second == 2)||(end.second - start.second == 1))){return true;}
      else{return false;}
      
    }
    //repeating above for black colored pawns.
    else if(this->to_ascii() =='p'){
      if(start.second-end.second == 1){return true;}
      else if((start.second == '7') && (start.second - end.second == 2)){return true;}
      else{return false;}
    }
    else{return false;}
  }
  //pawn changed columns; return false
  else{return false;}
}


bool Pawn::legal_capture_shape(std::pair<char, char> start, std::pair<char, char> end)const{
  //if pawn color is white
  if(this->to_ascii() == 'P'){
    //and the pawn moves one space left or right and one space forward it is valid
    if((std::abs(start.first-end.first)==1) && (end.second - start.second == 1)){return true;}
    else{return false;}
  }
  else{
    //the definition for forward is different for opposite colored piece.
    if((std::abs(start.first - end.first) == 1)&& (start.second - end.second == 1)){return true;}
    else{return false;}
  }
}
