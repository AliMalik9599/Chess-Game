CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -g

chess: main.o Bishop.o King.o Queen.o Pawn.o Rook.o Knight.o CreatePiece.o Board.o Chess.o
	$(CC) main.o Bishop.o King.o Queen.o Pawn.o Rook.o Knight.o CreatePiece.o Board.o Chess.o -o chess

main.o: main.cpp Chess.h
	$(CC) $(CFLAGS) -c main.cpp

Chess.o: Chess.cpp Chess.h Board.h Piece.h
	$(CC) $(CFLAGS) -c Chess.cpp

Board.o: Board.cpp Board.h Terminal.h CreatePiece.h
	$(CC) $(CFLAGS) -c Board.cpp

CreatePiece.o: CreatePiece.cpp CreatePiece.h Bishop.h King.h Queen.h Pawn.h Rook.h Knight.h Mystery.h
	$(CC) $(CFLAGS) -c CreatePiece.cpp

Bishop.o: Bishop.cpp Bishop.h Piece.h
	$(CC) $(CFLAGS) -c Bishop.cpp

King.o: King.cpp King.h Piece.h
	$(CC) $(CFLAGS) -c King.cpp

Queen.o: Queen.cpp Queen.h Piece.h
	$(CC) $(CFLAGS) -c Queen.cpp

Pawn.o: Pawn.cpp Pawn.h Piece.h
	$(CC) $(CFLAGS) -c Pawn.cpp

Rook.o: Rook.cpp Rook.h Piece.h
	$(CC) $(CFLAGS) -c Rook.cpp

Knight.o: Knight.cpp Knight.h Piece.h
	$(CC) $(CFLAGS) -c Knight.cpp
#make clean command
clean:
	rm -f *.o chess
