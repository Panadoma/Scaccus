#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "tc.h"
#include <stdlib.h>
#include "config.h"
#include <stdint.h>

typedef uint64_t BitBoard;
typedef enum {Pawn,King,Queen,Knight,Bishop,Rook,Empty} piece;
typedef enum {Black, White, wood} pieceColor;
typedef enum {insert, normal, visual} mode;

struct lookup{
	BitBoard AFile;
	BitBoard BFile;
	BitBoard CFile;
	BitBoard DFile;
	BitBoard EFile;
	BitBoard FFile;
	BitBoard GFile;
	BitBoard HFile;
	BitBoard firstRank ;
	BitBoard secondRank ;
	BitBoard thirdRank ;
	BitBoard fourthRank ;
	BitBoard fithRank ;
	BitBoard sixthRank ;
	BitBoard seventhRank ;
	BitBoard eighthRank ;

}lookupTable = {

	0b1000000010000000100000001000000010000000100000001000000010000000,
	0b0100000001000000010000000100000001000000010000000100000001000000,
	0b0010000000100000001000000010000000100000001000000010000000100000,
	0b0001000000010000000100000001000000010000000100000001000000010000,
	0b0000100000001000000010000000100000001000000010000000100000001000,	
	0b0000010000000100000001000000010000000100000001000000010000000100,
	0b0000001000000010000000100000001000000010000000100000001000000010,
	0b0000000100000001000000010000000100000001000000010000000100000001,
	0b1111111100000000000000000000000000000000000000000000000000000000,
	0b0000000011111111000000000000000000000000000000000000000000000000,
	0b0000000000000000111111110000000000000000000000000000000000000000,
	0b0000000000000000000000001111111100000000000000000000000000000000,
	0b0000000000000000000000000000000011111111000000000000000000000000,
	0b0000000000000000000000000000000000000000111111110000000000000000,
	0b0000000000000000000000000000000000000000000000001111111100000000,
	0b0000000000000000000000000000000000000000000000000000000011111111,
	
};

struct board{
	char turn	     ;	
	char wCastle         ;	
	char bCastle         ;	
	BitBoard wPawnBoard  ;
	BitBoard wRookBoard  ;
	BitBoard wBishopBoard;
	BitBoard wKnightBoard;
	BitBoard wQueenBoard ;
	BitBoard wKingBoard  ;
	BitBoard bPawnBoard  ;
	BitBoard bRookBoard  ;
	BitBoard bBishopBoard;
	BitBoard bKnightBoard;
	BitBoard bQueenBoard ;
	BitBoard bKingBoard  ;

	BitBoard wAttackedSquares;
	BitBoard bAttackedSquares;

	BitBoard pinnedPieces;
	
	// The display bitboards
	BitBoard pointer     ;

	BitBoard backColor1  ;
	BitBoard backColor2  ;
	BitBoard backClor3  ;

 	BitBoard wPieces;
 	BitBoard bPieces;
 	BitBoard foreColor1  ;

	BitBoard PieceCouldGo;   //Identifyes the empty squares that the pointer piece is attacking
	BitBoard PieceCouldCapture;  //Identifyes the non-empty enemy squares that the pointer piece is attacking 
				 //
}startingBoard = {
	0,1,1,
	0b0000000011111111000000000000000000000000000000000000000000000000,
	0b1000000100000000000000000000000000000000000000000000000000000000,
	0b0010010000000000000000000000000000000000000000000000000000000000,
	0b0100001000000000000000000000000000000000000000000000000000000000,
	0b0001000000000000000000000000000000000000000000000000000000000000,
	0b0000100000000000000000000000000000000000000000000000000000000000,
	0b0000000000000000000000000000000000000000000000001111111100000000,
	0b0000000000000000000000000000000000000000000000000000000010000001,
	0b0000000000000000000000000000000000000000000000000000000000100100,
	0b0000000000000000000000000000000000000000000000000000000001000010,
	0b0000000000000000000000000000000000000000000000000000000000010000,
	0b0000000000000000000000000000000000000000000000000000000000001000,
	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,

	0b0000000000000000000000000000000000000000000000000000000000000000,

	0b0000000000000000000000000000000000000000000000000000000000000001,	

	0b0101010101010101010101010101010101010101010101010101010101010101,
	0b0000000000000000000000000000000000000000000000001111111111111111,
	0b0000000000000000000000000000000000000000000000000000000000000000,
	
	0b1111111111111111000000000000000000000000000000000000000000000000,
	0b0000000000000000000000000000000000000000000000001111111111111111,
	0b0000000000000000000000000000000000000000000000000000000000000000,
},emptyBoard = {
	0,1,1,
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	
	0b0000000000000000000000000000000000000000000000000000000000000000,	

};





