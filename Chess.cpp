
#include "Chess.h"
#include<vector>
#include<string>

Chess::Chess() : is_white_turn(true) {
  // Add the pawns
  for (int i = 0; i < 8; i++) {
    board.add_piece(std::pair<char, char>('A' + i, '1' + 1), 'P');
    board.add_piece(std::pair<char, char>('A' + i, '1' + 6), 'p');
  }
  
  // Add the rooks
  board.add_piece(std::pair<char, char>( 'A'+0 , '1'+0 ) , 'R' );
  board.add_piece(std::pair<char, char>( 'A'+7 , '1'+0 ) , 'R' );
  board.add_piece(std::pair<char, char>( 'A'+0 , '1'+7 ) , 'r' );
  board.add_piece(std::pair<char, char>( 'A'+7 , '1'+7 ) , 'r' );
  
  // Add the knights
  board.add_piece(std::pair<char, char>( 'A'+1 , '1'+0 ) , 'N' );
  board.add_piece(std::pair<char, char>( 'A'+6 , '1'+0 ) , 'N' );
  board.add_piece(std::pair<char, char>( 'A'+1 , '1'+7 ) , 'n' );
  board.add_piece(std::pair<char, char>( 'A'+6 , '1'+7 ) , 'n' );
  
  // Add the bishops
  board.add_piece(std::pair<char, char>( 'A'+2 , '1'+0 ) , 'B' );
  board.add_piece(std::pair<char, char>( 'A'+5 , '1'+0 ) , 'B' );
  board.add_piece(std::pair<char, char>( 'A'+2 , '1'+7 ) , 'b' );
  board.add_piece(std::pair<char, char>( 'A'+5 , '1'+7 ) , 'b' );
  
  // Add the kings and queens
  board.add_piece(std::pair<char, char>( 'A'+3 , '1'+0 ) , 'Q' );
  board.add_piece(std::pair<char, char>( 'A'+4 , '1'+0 ) , 'K' );
  board.add_piece(std::pair<char, char>( 'A'+3 , '1'+7 ) , 'q' );
  board.add_piece(std::pair<char, char>( 'A'+4 , '1'+7 ) , 'k' );
}

Chess::Chess(const Board& insboard, bool white):board(insboard), is_white_turn(white){}
Chess::~Chess(){}

// Returns true if the position is within bounds
bool Chess::within_bounds(std::pair<char, char> position) const {
  // must be between A-H and '1'-'8'.
  if((position.first >= 'A' && position.first <= 'H') && (position.second >= '1' && position.second <= '8')){
    return true;
  } else {
    return false;
  }
}


//makes the move on a copied board to see if it is legal.
//this needs to be done on copied board to see if the move would put king in check.
std::string Chess::pseudo_move(std::pair<char, char> start, std::pair<char, char> end) const {
  std::string msg;  
  // Board copied_board(this->board);
  Chess copied_chess(this->board, this->turn_white());
  const Piece* piece_end = copied_chess.board(end);
  const Piece* piece = copied_chess.board(start);
  int dx = std::abs(end.first - start.first);
  int dy = std::abs(end.second - start.second);
  //making sure that the start and end positions are within boundaries
  if (!within_bounds(start) || !within_bounds(end)) {
    msg = "This move is out of bounds";
    return msg;
  }
  
  //Check there is a piece at start position
  if (!piece) {
    msg = "No piece at the starting location";
    return msg;
  }
  //there is a piece, so getting its designator
  char designator = piece->to_ascii();

  
  //prevents other player from moving opposite color piece
  if (piece->is_white() && !turn_white()) {
    msg = "Not your piece, that's a white piece";
    return msg;
  }

  //same as above (specified error message to whos turn it is)
  if (!(piece->is_white()) && turn_white()) {
    msg = "Not your piece, that's a black piece";
    return msg;
  }

  //Check that start pos != end pos and that piece actually moves
  if (board(start) == board(end)) {
    msg = "The specified start and end positions are the same";
    return msg;
  }

  //checks to make sure the move shape is legal or the capture shape is legal.
  bool legality= piece->legal_move_shape(start, end)||piece->legal_capture_shape(start,end);

  //neither a valid move shape or capture shape
  if (legality == false) {
    msg = "This is an illegal move";
    return msg;
  }

  //checking to see if the piece is moving over other pieces in its move shape.
  if(legality){
    //Knights may jump over other pieces, exclude these cases.
    //if(designator != 'N' && designator !='n')
    if(dx==0 || dy == 0 || dx == dy){
      //in_the_way returns true if there is a piece in the way of the given move.
      bool obstructed = in_the_way(start,end);
      if(obstructed){
	msg = "This piece may jump over other pieces.";
	return msg;
      }
    }
  }

  //making sure that own piece is not attempted to be captured.
  if(piece_end && piece_end->is_white()==piece->is_white()){
    msg = "You can not take your own piece";
    return msg;
  }

  //dealing with the special attack pattern of pawns.
  if(designator=='p' || designator=='P'){
    bool move = piece->legal_move_shape(start,end);
    if(move && piece_end){
      msg = "Pawns can not attack in that direction";
      return msg;
    }
    //if not move, then the above legality was evaluated true for legal capture shape.
    //i.e. the pawn must be moving diagonally here
    //which is not okay if there is no piece at the end of the move.
    else if(!move && !piece_end){
      msg = "Pawns may only move diagonally when attacking";
      return msg;
    }
  }

  //making the move
  copied_chess.board.remove_piece(end);
  copied_chess.board.add_piece(end, designator);
  copied_chess.board.remove_piece(start);
  //  copied_chess.board.add_piece(start, '\0');

  //checking to see if that move resulted in putting own king in check
  //if so, undo the move return error message
  if(copied_chess.in_check(copied_chess.turn_white())){
    
    // copied_chess.board.add_piece(start, designator);
    //copied_chess.board.add_piece(end,'\0');
    msg = "You can not put yourself in check";
    return msg;
  }
  //undo the move here because this is only a pseudo move

  copied_chess.board.remove_piece(end);
  copied_chess.board.add_piece(end,designator);
  copied_chess.board.remove_piece(start);
  msg = "Successful move";
  return msg;
}

//this function makes a move or prints an error message out if the move is invalid
bool Chess::make_move(std::pair<char, char> start, std::pair<char, char> end){
  std::string check;
  check = pseudo_move(start, end);
  //if psuedo_move was okay, then make the move on the real board.
  if(check.compare("Successful move") == 0){
    const Piece* piece = this->board(start);
    char designator = piece->to_ascii();
    

    
    //if it is a pawn, then upgrade the pawn if it is on the last row
    //of the opposite color
    if(designator =='P' && end.second =='8'){

      this->board.remove_piece(end);
      this->board.add_piece(end,'Q');
      this->board.remove_piece(start);
  
      //same as above for the opposite color pawn
  } else if (designator == 'p' && end.second=='1') {
      this->board.remove_piece(end);
      this->board.add_piece(end,'q');
      this->board.remove_piece(start);

      //handling adding regular pieces
  } else {
      this->board.remove_piece(end);
      this->board.add_piece(end,designator);
      this->board.remove_piece(start);
    }
    //switching up the turns
    if(turn_white()){
      this->is_white_turn = false;
    } else {
      this->is_white_turn = true;
    }
    return true;
    //if it was unsuccessful print the error message.
  } else {
    std::cerr<<check<<std::endl;
    return false;
  }
}


bool Chess::in_check(bool white) const {
  std::pair<char,char> attacked_pos;
  //finding the location of the king potentially in check
  for(char i = 'A'; i<'I'; i++){
    for(char j = '1'; j<'9'; j++){
      std::pair<char,char> boarditer = std::make_pair(i,j);
      if(this->board(boarditer)){
	if(white && this->board(boarditer)->to_ascii() == 'K'){
	  attacked_pos = boarditer;
	  break;
	}
	else if (!white && this->board(boarditer)->to_ascii() == 'k'){
	  attacked_pos = boarditer;
	  break;
	}
      }
    }
  }

  //checking all directions from the king's location to determine if it is under attack by rook, bishop, or queen.
  for(int dy = -1; dy<=1; dy++){
    for(int dx = -1; dx<=1; dx++){
      //defining the position that I will look to see if check is occuring from.
      std::pair<char,char> check_pos = std::make_pair(attacked_pos.first, attacked_pos.second);
      //checking dx = 0, dy = 0 results in the below while loop looping infinitely, I need to exclude that case.
      if(dx==0 && dy ==0){continue;}
      //checking to the edge of the board in the direction of dx, dy with the while loop.
      while(within_bounds(check_pos)){
	
        check_pos.second+=dx;
        check_pos.first+=dy;
	if(!within_bounds(check_pos)){
	  break;
	}
        //if either of these are equal to zero the position I will be checking is up, down, right, or left from the king.
        //pieces that threaten on diagonals are rooks and queens.
        if(dx==0 || dy==0){
	  //checking if black king is in check by straight attack
          if(!white){
            //if there is no piece on the square check the next square out (reiterate the while loop.)
            if(this->board(check_pos)==nullptr){continue;}
            else if((this->board(check_pos)->to_ascii()=='Q' ||
                     this->board(check_pos)->to_ascii() == 'R')){
              return true;
            }
            //if there is a piece that is not a rook or queen inbetween them and the king the king is not in check.
            else{break;}
          }
          //doing the same if white == true.
          else{
            if(this->board(check_pos)==nullptr){continue;}
            else if((this->board(check_pos)->to_ascii()=='q' ||
                     this->board(check_pos)->to_ascii() == 'r')){
              return true;
            }
            else{break;}
          }
	  
        }
        //if dx and dy != 0, then the positions I will be checking are the diagonals.
        //pieces that threaten on diagonals are queens and bishops
        else{
          //checking to see if black king is in check by diagonal attack
          if(!white){
            //if empty square, continue
            if(this->board(check_pos) == nullptr){continue;}
            //if square contains white bishop or queen, the king is checked
            else if((this->board(check_pos)->to_ascii() =='Q' ||
                     this->board(check_pos)->to_ascii() =='B')){
              return true;
            }
            //if it is a piece other than a bishop or queen, then it blocks the check from hitting the king
            else{break;}
          }
          //doing the same thing if we are checking the white king.
          else{
            if(this->board(check_pos) == nullptr){continue;}
            else if((this->board(check_pos)->to_ascii() == 'q' ||
                     this->board(check_pos)->to_ascii() == 'b'))
	      return true;
	    else{break;}
	  }
	}
      }
    }
  }
 
  //now checking the special case of knights and pawns.
  //first doing the knights.
  for(int i =-2; i<=2; i++){
    for(int j = -2; j<=2; j++){
      if(i==j || i==0 || j==0){continue;}
      //if a knight is on this position, then king is checked
      std::pair<char,char> knight_pos = std::make_pair(attacked_pos.first+i, attacked_pos.second+j);
      if(this->board(knight_pos)){
	if(white){
	  if(this->board(knight_pos)->to_ascii()=='n'){
	    return true;
	  }
	}
	//repeating for the opposite color
	else{
	  if(this->board(knight_pos)->to_ascii()=='N'){
	    return true;
	  }
	}
      }
      else{continue;}
    }
  }
  //now checking the pawns.
  if(white){
    //if a pawn of the opposite color is in either of the below positions, the king is in check.
    std::pair<char,char> pawn1 = std::make_pair(attacked_pos.first+1,attacked_pos.second+1);
    std::pair<char,char> pawn2 = std::make_pair(attacked_pos.first-1,attacked_pos.second+1);
    if(this->board(pawn1)){
      if(this->board(pawn1)->to_ascii()=='p'){return true;}
    }
    else if(this->board(pawn2)){
      if(this->board(pawn2)->to_ascii()=='p'){return true;}
    }
  }
  //repeating for the opposite color.
  else if(!white){
    std::pair<char,char> pawn1 = std::make_pair(attacked_pos.first+1,attacked_pos.second-1);
    std::pair<char,char> pawn2 = std::make_pair(attacked_pos.first-1,attacked_pos.second-1);
    if(this->board(pawn1)){
      if(this->board(pawn1)->to_ascii()=='P'){return true;}
    }
    else if(this->board(pawn2)){
      if(this->board(pawn2)->to_ascii() == 'P'){return true;}
    }
  }
  
  //now checking the enemy king.
  for(int dx = -1; dx<=1;dx++){
    for(int dy = -1; dy<=1; dy++){
      if(dx==0 && dy == 0){continue;}
      //if an enemy king is in this position, then the king we are looking at is in check.
      std::pair<char,char> enemy_king_pos = std::make_pair(attacked_pos.first + dx, attacked_pos.second + dy);
      if(white){
	if(this->board(enemy_king_pos) && this->board(enemy_king_pos)->to_ascii() == 'k'){return true;}
      }
      //doing the same for the opposite color king.
      else{
	if(this->board(enemy_king_pos) && this->board(enemy_king_pos)->to_ascii() == 'K'){return true;}
      }
    }
  }

  //now checking the mystery piece...
  //first i find the mystery piece
  //because I don't premptively know the shape of a move for a mystery piece I do it slightly differently:
  //iterating over all possible board positions to try and find a mystery piece.
 for(char r = '8'; r >= '1'; r--) {
    for(char c = 'A'; c <= 'H'; c++) {
      std::pair<char,char> iterpos = std::make_pair(c,r);
      //dx and dy are neccessary to tell what kind of move the mystery piece is making.
      int myst_dx;
      int myst_dy;
      if(white){
	//if non null
	if(this->board(iterpos)){
	  //if it is a mystery piece
	  if(this->board(iterpos)->to_ascii()=='m'){
	    //finding the valid move shape for the mystery piece
	    if(this->board(iterpos)->legal_move_shape(iterpos,attacked_pos)){
	      //if it was valid, find out what dx and dy were
	      myst_dx = c - 'A';
	      myst_dy = r -'1';
	      //mystery piece moves like a knight and can jump pieces
	      if(myst_dx!=myst_dy && myst_dx !=0 && myst_dy != 0){
		return true;
	      }
	      //mystery piece doesn't move like a knight, but nothing is in the
	      //way of it, so it still checks king.
	      else{
		if(!(this->in_the_way(iterpos,attacked_pos))){return true;}
	      }
	    }
	  }
	}
      }
      //same thing, but for black pieces.
      else{
	if(this->board(iterpos)){
	  if(this->board(iterpos)->to_ascii()=='M'){
	    if(this->board(iterpos)->legal_move_shape(iterpos,attacked_pos)){
	      myst_dx = c - 'A';
	      myst_dy = r -'1';
	      //mystery piece moves like a knight and can jump pieces
	      if(myst_dx!=myst_dy && myst_dx !=0 && myst_dy != 0){
		return true;
	      }
	      else{
		if(!(this->in_the_way(iterpos,attacked_pos))){return true;}
	      }	      
	    }
	  }
	}
      }
    }
 }
	  
  //if the function hasn't returned yet, (white) is not in check.
  return false;
}




bool Chess::in_mate(bool white) const {
  if (white) {
    //making sure that white is in check
    bool valid = in_check(white);
    //if (white) is, checking to see if there are any moves for white.
    if (valid) {
      //returns false if there are no moves, true if there is.
      return mate_helper_white();
    }
    //code for opposite color
  }else {
    bool valid = in_check(white);
    if (valid) {
      return mate_helper_black();
    }
    
  }
  return false;
}

bool Chess::mate_helper_white() const{
  //iterating over all cols.
    for(char r = '8'; r >= '1'; r--) {
      //over all rows
      for(char c = 'A'; c <= 'H'; c++) {
	//checking if the piece at this location is non-null and white.
	const Piece* piece = this->board(std::pair<char, char>(c, r));
	if (piece != nullptr) {
	  if (piece->to_ascii() >= 'A' && piece->to_ascii() <= 'Z') {
	    std::string move;
	    for(char j = '8'; j >= '1'; j--) {
	      for(char k = 'A'; k <= 'H'; k++) {
		//checking all possible places that this piece may move.
		std::pair<char,char> start = std::make_pair(c,r);
		std::pair<char,char> end = std::make_pair(k,j);
		move = pseudo_move(start, end);
		//if there is a move that results in white not being in check,
		//white is not in checkmate.
		if(move.compare("Successful move")==0){
		  return false;
		}
	      }
	    }
	  }
	}
      }
    }
    return true;
}
//identical to mate_helper_white(), but for black pieces.
bool Chess::mate_helper_black() const{
  for(char r = '8'; r >= '1'; r--) {
    for(char c = 'A'; c <= 'H'; c++) {
      const Piece* piece = this->board(std::pair<char, char>(c, r));
      if (piece != nullptr) {
	if (piece->to_ascii() >= 'a' && piece->to_ascii() <= 'z') {
	  std::string move;
	  for(char j = '8'; j >= '1'; j--) {
	    for(char k = 'A'; k <= 'H'; k++) {
	      std::pair<char,char> start = std::make_pair(c,r);
	      std::pair<char,char> end = std::make_pair(k,j);
	      move = pseudo_move(start, end);
	      if(move.compare("Successful move")==0){
		return false;
	      }
	    }
	  }
	}
      }
    }
  }
  return true;
}

//checks to see if (white) is in stalemate
bool Chess::in_stalemate(bool white) const {
  if (white) {
    //if (white) is in check, then (white) may not be in stalemate
    bool valid = in_check(white);
    if (valid) {
      return false;
    }
    //checking to see if there exists valid moves for white
    return mate_helper_white();
    //doing the same for the opposite color.
  } else {
    bool valid = in_check(white);
    if (valid) {
      return false;
    }
    return mate_helper_black();  
  }
  return false;
}

//function that checks to see if there are pieces in the way of a move
bool Chess::in_the_way(std::pair<char,char> start, std::pair<char,char> end)const {
  //finding the direction of the move
  int dy = end.second - start.second;
  int dx = end.first - start.first;
  //normalizing the direction so that I may use it as an iterator variable
  //it is okay to divide both by the same because if it is diagonal they are the same value
  //if not one is zero, so it isn't scaled differently.
  if (dx!=0){
    dy/=abs(dx);
    dx/=abs(dx);
  }
  else{
    dx/=abs(dy);
    dy/=abs(dy);
  }
  //checking all positions between the start and end position (exclusive of the end position
  //end position is exclusive because it is okay for piece of opposite color to be there.
  std::pair<char,char> iterpos = std::make_pair(start.first,start.second);
  while(iterpos!=std::pair<char,char>(end.first-dx,end.second-dy)){
    iterpos.first+=dx;
    iterpos.second+=dy;
    if(this->board(iterpos)){
      return true;
    }
  }
  return false;
}


void Chess::clear_board(){
  for(int i = 7; i>=0; i--){
    for(int j = 0; j<8; j++){
      std::pair<char,char> pos = std::make_pair('A'+j,'1'+i);
      this->board.remove_piece(pos);
    }
  }
}


//adding in piece at loc, designator. This was necessary to have priveledges
//without making chess a friend class of board.
bool Chess::add_load(std::pair<char,char> loc, char designator){
  bool truth = this->board.add_piece(loc,designator);
  return truth;
}

//changes the turn of who is going
void Chess::change_turn(char turn){
  if(turn == 'w'){
    this->is_white_turn = true;
  }else{
    this->is_white_turn = false;
  }
}
std::ostream& operator<< (std::ostream& os, const Chess& chess) {
  // Write the board out and then either the character 'w' or the character 'b',
  // depending on whose turn it is
  return os << chess.get_board() << (chess.turn_white() ? 'w' : 'b');
}


//loads in a board from a file. loaded boards have the form
//--------
//--------
//--------
//--------
//--------
//--------
//--------
//--------
//where "-" may be replaced by pieces.
std::istream& operator>> (std::istream& is, Chess& chess) {
  std::string line;
  std::vector<std::string> linevect;
  chess.clear_board();
  //reading in data
  while(std::getline(is,line)){
    linevect.push_back(line);
  }
  //finding out whose turn it is
  std::string turn = linevect[8];
  linevect.erase(linevect.begin()+8);
  //getting the turn in terms of a char.
  char t = turn[0];
  //iterating the rest of the vector to read it
  for(int i = 7; i>=0; i--){
    for(int j = 0; j<8; j++){
      std::pair<char,char> addpos = std::make_pair('A'+j,'1'+i);
      
      //adding anything that isn't a blank. (but blank shoudl be taken care of in createpiece functionality)
      if(linevect[7-i][j]!='-'){
	chess.add_load(addpos,linevect[7-i][j]);
      }
    }
  }
  //changing the turn to the color.
  chess.change_turn(t);
  return is;
}
