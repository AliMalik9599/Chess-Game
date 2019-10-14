
#include <iostream>
#include <utility>
#include <map>
#include "Board.h"
#include "CreatePiece.h"
#include "Terminal.h"


Board::Board(){}

//board copy constructor to be used in stalemate and checkmate.
Board::Board(const Board &orig){
  for(std::map<std::pair<char, char>, Piece *>::const_iterator it = orig.occ.cbegin(); it!=orig.occ.cend(); it++) {
    if(it->second){
      char designator = it->second->to_ascii();
      this->add_piece(it->first,designator);
    }//make a deep copy of the current board
  }
}

Board::~Board(){
  for(std::map<std::pair<char, char>, Piece *>::iterator it = this->occ.begin(); it!=this->occ.end(); it++) {
    if(it->second){
      delete it->second;
    }
  }
}

void Board::remove_piece(std::pair<char,char> location){
  if(this->occ.find(location) != this->occ.end()){
    delete this->occ[location];
    this->occ.erase(location);
  }
}

const Piece* Board::operator()(std::pair<char, char> position) const {
  //check if piece at position exists
  try{ 
    return this->occ.at(position);  
  }
  //if not then return null
  catch(const std::out_of_range &e){
    return nullptr;
  }
}

bool Board::add_piece(std::pair<char, char> position, char piece_designator) {
  if(position.first < 'A' || position.first > 'H'){ // making sure first positional argument is A-H
  	std::cerr << "not add-- wrong letters" << std::endl;
  	return false;
  } 
  if(position.second < '1' || position.second > '8'){// making sure second positional argument is 1-8
  	std::cerr << "not added-- wrong numbers" << std::endl;
  	return false;
  } 
  if(this->occ.find(position) != this->occ.end()) { // checking to see if piece already exists at this location.
  	std::cerr << "not added-- piece already exists at this location" << std::endl;
  	return false;
  }
  Piece* add_piece = create_piece(piece_designator);
  if(add_piece == nullptr){ //used to make starting positions point to null after a move
  	this->occ[position] = nullptr;
  	return false;
  }else if(add_piece){ //updating the position on the board to contain add_piece.
    this->occ[position] = add_piece;
    //std::cout << "piece added successfully" << std::endl;
    return true;
  }
  return false;
}

bool Board::has_valid_kings() const {
  int count_black = 0;
  int count_white = 0;
  for(std::map<std::pair<char, char>, Piece *>::const_iterator it = this->occ.cbegin(); it!=this->occ.cend(); it++) {
    if(it->second){	//iterator to go through all points on board and see if piece is present
      if (it->second->to_ascii() == 'K') {	//checking if white king exists, update counter
	count_white++;
      }
      if (it->second->to_ascii() == 'k') {	//check if black king exists, update counter
	count_black++;
      }
    }
  }
  
  if (count_white != 1 || count_black != 1) {	//return false if there isn't one black and one white king
    return false;
  }
  return true;
}


std::ostream& operator<<(std::ostream& os, const Board& board) {
  for(char r = '8'; r >= '1'; r--) {
    for(char c = 'A'; c <= 'H'; c++) {
      const Piece* piece = board(std::pair<char, char>(c, r));
      if (piece != nullptr) {
	os << piece->to_ascii();
      } else {
	os << '-';
      }
    }
    os << std::endl;
  }
  return os;
}

//chess pieces use unicode characters and change based
//on it's piece desginator
std::string Board::symbol(const Piece* piece) const{
  switch (piece->to_ascii()) {
	case 'K': return "\u2654";
	case 'k': return "\u265A";
	case 'Q': return "\u2655";
	case 'q': return "\u265B";
	case 'B': return "\u2657";
	case 'b': return "\u265D";
	case 'N': return "\u2658";
	case 'n': return "\u265E";
	case 'R': return "\u2656";
	case 'r': return "\u265C";
	case 'P': return "\u2659";
	case 'p': return "\u265F";
	case 'M': return "M";
	case 'm': return "m";
	default: return NULL;
	}
}

//print columns of the board
void print_cols(){
	std::cout << "    " << 'A' << "  " << 'B' << "  "  <<  'C'; 
	std::cout << "  " << 'D' << "  " << 'E' <<  "  " << 'F';
	std::cout << "  " << 'G' << "  " << 'H';
}

//Print a fancier board with colors and unicode chess
//as opposed to the save file format
void Board::display() const{
  //start with default terminal color
  Terminal::set_default();
  
  //checkered colors
  std::cout << std::endl;
  Terminal::Color c1 = Terminal::BLUE;
  Terminal::Color c2 = Terminal::WHITE;
  Terminal::Color black = Terminal::BLACK;

  print_cols();
  std::cout << std::endl;
  for(char row = '8'; row >= '1'; row--) {
  	  //switch colors when it's a new row
	  Terminal::Color tempc = c1;
      c1 = c2;
      c2 = tempc;
      Terminal::set_default();
      std::cout << " " << row << " ";
	  for (int col = 'A'; col <= 'H'; col++) {
	      Terminal::color_bg(c1);
	      const Piece* piece = this->operator()(std::pair<char, char>(col, row));
	      if (piece == nullptr) {
			std::cout << "   ";
	      } else {
		  		if (piece->is_white()) {
		  			//chess pieces use unicode characters and change the foreground
		  			Terminal::color_fg(true, black);
		  			std::cout << " " << symbol(piece) << " ";
		  		} else {
		  			//chess pieces use unicode characters and change the foreground
		  			Terminal::color_fg(true, black);
		  			std::cout << " " << symbol(piece) << " ";
		  		}
	      }
	      //switch when it's a new column
	      Terminal::Color tempc = c1;
	      c1 = c2;
	      c2 = tempc;
	    }
	    Terminal::set_default();
	    std::cout << " " << row << std::endl;
	    //std::cout << std::endl;
  }
  //end with default terminal color
  Terminal::set_default();
  print_cols();
  std::cout << std::endl;
}
