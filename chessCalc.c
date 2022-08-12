#include "scaccus.h"
#include "chessConversion.c"


void MovePointer(struct board *_board, char operation); // operation: h=0 j=1 k=2 l=3,					//
void UpdateBoard(struct board *_board);
void IdentifyPiece(struct board *_board,piece *p,pieceColor *pc,char *isPointer, char x, char y);
BitBoard KnightAttack(BitBoard _pos);
void Shout(char *message){
//print a message on another temrinal
}
void MovePointer(struct board *_board, char operation){

        char shift =0;
        switch (operation){
                case 0: //h key press
                        shift = 1;
                        break;

                case 1: //j key press
                        shift = -8;
                         break;

                case 2: //k key press
                        shift = 8;
                         break;

                case 3: //l key press
                        shift = -1;
                         break;
        }
        BitBoard  y = (_board->pointer << shift) | (_board->pointer >> (64ull - shift));
        _board->pointer = y;
}


void PrintBitBoard(BitBoard _bitBoard){  // For debugging, print a Bitboard

        char pos;
        for(char i=7; i>=0; --i){
                for(char j=0; j<8; ++j){

                        pos= 63 - (j + 8*i);
                        BitBoard mask = 1ULL <<pos;

                        if(mask &_bitBoard){

                                printf(" ");
                                printf(_empty_square_);
                        }
                        else{

                                printf(" .");
                        }
        }


        printf("\n");
        }
}

uint8_t GetFile(BitBoard _board, char file){

	uint8_t returnValue=0Ull;
	char index =0;
	for(char i=(63-file);i>=0;i-=8){
		returnValue = returnValue | ((_board>>i)&1Ull)<<index++;
	}
	return returnValue;
}


uint8_t GetRank(BitBoard _board, char rank){

	uint8_t returnValue=0Ull;
	char index =7;
	rank = 7 -rank;
	rank*=8;
	for(char i=7+rank;i>=rank;i--){
		returnValue = returnValue | ((_board>>i)&1Ull)<<index--;
	}
	return returnValue;
}

BitBoard KnightAttack(BitBoard _pos){

	BitBoard returnValue = 0ULL;
	returnValue = returnValue | 
		(_pos<<10)&~lookupTable.GFile&~lookupTable.HFile |
		(_pos<<6) &~lookupTable.AFile &~lookupTable.BFile|
		(_pos<<17)&~lookupTable.HFile|
		(_pos<<15)&~lookupTable.AFile| 
		(_pos>>10)&~lookupTable.AFile&~lookupTable.BFile|
		(_pos>>6) &~lookupTable.GFile&~lookupTable.HFile| 
		(_pos>>17)&~lookupTable.AFile| 
		(_pos>>15)&~lookupTable.HFile;
	return returnValue;
}

void GetCoordinates(char *i,char *j, BitBoard pointer){
	char index=-1;
	while(pointer){
		++index;
		pointer= pointer<<1;
	}
	*i=(index%8);
	*j=(index)/8;
}

void PointingAtAKnight(struct board *_board,char bOrW){
	
	if(!(_board->pinnedPieces&_board->pointer)){  //ie if the piece is not pineed
		BitBoard rawAttackBoard= KnightAttack(_board->pointer);
		BitBoard EnemyBoard = bOrW ? _board->bPieces : _board->wPieces;
		BitBoard FriendBoard = bOrW ? _board->wPieces : _board->bPieces;
		BitBoard canMoveBoard = rawAttackBoard & ~FriendBoard &~EnemyBoard; 
		BitBoard canCaptureBoard = rawAttackBoard &EnemyBoard;
		_board->PieceCouldGo = canMoveBoard;
		_board->PieceCouldCapture= canCaptureBoard;
	}	

}

void PointingAtAPawn(struct board *_board,char bOrW){ //0 is white

	char canMove=1; 
	BitBoard returnValue=0Ull;	
	if(_board->pinnedPieces&_board->pointer){
		canMove= 0	;
	}
	
	if(_board->pointer&lookupTable.secondRank&&bOrW)	{ 

		Shout("Its a whitePawn, on the second rank\n");	

	}	
	if(_board->pointer&lookupTable.seventhRank&&!bOrW)	{

		
		Shout("Its a blackPawn, on the senveth rank\n");	
	}
	
}
uint8_t ReverseBits(uint8_t num)
{
    uint8_t NO_OF_BITS = sizeof(num) * 8;
    uint8_t reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}
void IdentifyPiece(struct board *_board,piece* p, pieceColor* pc, char* isPointer, char y, char x){

        char pos= 63 - (x + 8*y);
        BitBoard mask = 1ULL <<pos;
        if(_board->pointer&mask){
                *isPointer = 1;
        }
        else *isPointer =0;
        *p = Empty;
        *pc = wood;
        if(_board->wPawnBoard&mask){
                *p = Pawn;
                *pc = White;
                goto lblColors;
        }
        if(_board->wKnightBoard&mask){
                *p = Knight;
                *pc = White;
                goto lblColors;
        }
        if(_board->wKingBoard&mask){
                *p = King;
                *pc = White;
                goto lblColors;
        }
        if(_board->wQueenBoard&mask){
                *p = Queen;
                *pc = White;
                goto lblColors;
        }
        if(_board->wBishopBoard&mask){
                *p = Bishop;
                *pc = White;
                goto lblColors;
        }
        if(_board->wRookBoard&mask){
                *p = Rook;
                *pc = White;
                goto lblColors;
        }
        if(_board->bPawnBoard&mask){
                *p = Pawn;
                *pc = Black;
                goto lblColors;
        }
        if(_board->bKnightBoard&mask){
                *p = Knight;
                *pc = Black;
                goto lblColors;
        }
        if(_board->bKingBoard&mask){
                *p = King;
                *pc = Black;
                goto lblColors;
        }
        if(_board->bQueenBoard&mask){
                *p = Queen;
                *pc = Black;
                goto lblColors;
        }
        if(_board->bBishopBoard&mask){
                *p = Bishop;
                *pc = Black;
                goto lblColors;
        }
        if(_board->bRookBoard&mask){
                *p = Rook;
                *pc = Black;
                goto lblColors;
        }
        lblColors:
}
uint8_t RayFillLeft(uint8_t input){ 
	char index=7;
	uint8_t returnValue=0;
	while(!((input>>index)&1)&&index>=0){
		returnValue =returnValue|1<<index;
		--index;
	}
	return returnValue;
}

uint8_t RayFill(uint8_t enemy,uint8_t blockers, uint8_t *empty, uint8_t *capture  ){ 
	unsigned char index=0;
	uint8_t emptySquares=0;
	//printf("%d\n",((blockers>>index)&1));
	while((!((blockers>>index)&1))&&index<9){
		emptySquares=emptySquares|(1<<index);
		++index;
	}
	if(enemy>>index&1){
		*capture = (*capture)|(1<<index);
	}
	*empty=emptySquares;
}
void PointingAtARook(struct board *_board,char bOrW){

		BitBoard enemyBoard = bOrW ? _board->bPieces : _board->wPieces;
		BitBoard blockerBoard = bOrW ? _board->wPieces|enemyBoard : _board->bPieces|enemyBoard;
		BitBoard emptySquare =0Ull;
		BitBoard canCapture =0Ull;
		char x,y;
		GetCoordinates(&x,&y,_board->pointer);		
		for(int i=(6-x);i>=0;i--){ //right ray
			BitBoard tmp = (1Ull<<(((7-y)*8 + (i))));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}	
		for(int i=x-1;i>=0;i--){ //left ray
			
			BitBoard tmp = (1Ull<<((7-y)*8 + (7-i) ));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}	
		for(int i=y-1;i>=0;i--){ //down
			BitBoard tmp = (1Ull<<((7-i)*8 + (7-x)));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}
		for(int i=(6-y);i>=0;i--){ //up

			BitBoard tmp = (1Ull<<((i)*8 + (7-x)));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}
		_board->PieceCouldGo = emptySquare;
		_board->PieceCouldCapture= canCapture;
}

void PointingAtBishop(struct board *_board,char bOrW){

		BitBoard enemyBoard = bOrW ? _board->bPieces : _board->wPieces;
		BitBoard blockerBoard = bOrW ? _board->wPieces|enemyBoard : _board->bPieces|enemyBoard;
		BitBoard emptySquare =0Ull;
		BitBoard canCapture =0Ull;
		char x,y;
		GetCoordinates(&x,&y,_board->pointer);	
		char largerCoordinate = x >= y ? x : y;
		for(int i=1;i<=7-largerCoordinate;++i){ //north-east	
			BitBoard tmp = (1Ull<<63-(((y+i)*8 + (x+i))));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}
		largerCoordinate = (7-x) >= y ? (7-x) : y;
		for(int i=1;i<= 7-largerCoordinate;++i){ //north-west
			
			BitBoard tmp = (1Ull<<63-((y+i)*8 + (x-i)));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{	
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}
		largerCoordinate = (7-x) >= (7-y) ? (7-x) : (7-y);
		for(int i=1;i<=7- largerCoordinate;++i){ //south-west
			BitBoard tmp = (1Ull<<63-(((y-i)*8 + x - i)));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}
		largerCoordinate = x >= 7-y ? x : 7-y;
		for(int i=1;i<= 7-largerCoordinate;++i){ //south-east
			
			BitBoard tmp = (1Ull<<63-((y-i)*8 + (x+i)));
			if(!(tmp&blockerBoard))
				emptySquare = emptySquare|tmp;
			else{	
				if(tmp&enemyBoard){
					canCapture = canCapture|tmp;
				}
				break;
			}
		}
		_board->PieceCouldGo = emptySquare;
		_board->PieceCouldCapture= canCapture;
}

void UpdateBoard(struct board *_board){
/*
	printf("\n");
	PrintBitBoard(emptyBoard.pointer);
	PointingAtARook(_board,1);	

	*/
	_board->pointer = 1Ull<<(8*5)+5;
	PrintBitBoard(emptyBoard.pointer);
	printf("\n");
	PointingAtBishop(&emptyBoard,0);
	printf("\n");
	
}



