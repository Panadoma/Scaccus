#include "scaccus.h"


void MovePointer(struct board *_board, char operation); // operation: h=0 j=1 k=2 l=3,					//
void UpdateBoard(struct board *_board);
void IdentifyPiece(char* PieceID,struct board *_board,piece *p,pieceColor *pc,char *isPointer, char x, char y);
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

void InitPieceArray(struct board *_board){
	pieceArray[0] = &(_board->wPawnBoard);
	pieceArray[1] = &(_board->wRookBoard);
	pieceArray[2] = &(_board->wKnightBoard);
	pieceArray[3] = &(_board->wBishopBoard);
	pieceArray[4] = &(_board->wKingBoard);
	pieceArray[5] = &(_board->wQueenBoard);
	pieceArray[6] = &(_board->bPawnBoard);
	pieceArray[7] = &(_board->bRookBoard);
	pieceArray[8] = &(_board->bKnightBoard);
	pieceArray[9] = &(_board->bBishopBoard);
	pieceArray[10]= &(_board->bKingBoard);
	pieceArray[11]= &(_board->bQueenBoard);
}

void PrintBitBoard(BitBoard _bitBoard){  // For debugging, print a Bitboard

        char pos;
        printf("\n");
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

void Move(struct board *_board,BitBoard pos1, BitBoard pos2){
	//IsLegal();
	InitPieceArray(_board); //To access the pieces of the board as an array 
	char currentPieceIndex=0;
	for(int i=0;i<11;++i){
		if(*pieceArray[i]&pos1){
			currentPieceIndex = i;
			break;
		}
	}

	if(*pieceArray[currentPieceIndex]&(_board->wPieces)){


		_board->wPieces = _board->wPieces^pos1;	
		_board->wPieces = _board->wPieces|pos2;	
	}
	else{
		_board->bPieces =  _board->bPieces^pos1;	
		_board->bPieces =  _board->bPieces|pos2;	
	}

	*pieceArray[currentPieceIndex] = *pieceArray[currentPieceIndex]^pos1;
	*pieceArray[currentPieceIndex] = *pieceArray[currentPieceIndex] | pos2;

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
void IdentifyPiece(char* PieceID, struct board *_board,piece* p, pieceColor* pc, char* isPointer, char y, char x){

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
		*PieceID = 0;
                goto lblColors;
        }
        if(_board->wKnightBoard&mask){
                *p = Knight;
                *pc = White;
		*PieceID = 1;
                goto lblColors;
                goto lblColors;
        }
        if(_board->wKingBoard&mask){
                *p = King;
                *pc = White;
		*PieceID = 2;
                goto lblColors;
                goto lblColors;
        }
        if(_board->wQueenBoard&mask){
                *p = Queen;
                *pc = White;
		*PieceID = 3;
                goto lblColors;
                goto lblColors;
        }
        if(_board->wBishopBoard&mask){
                *p = Bishop;
                *pc = White;
		*PieceID = 4;
                goto lblColors;
                goto lblColors;
        }
        if(_board->wRookBoard&mask){
                *p = Rook;
                *pc = White;
		*PieceID = 5;
                goto lblColors;
                goto lblColors;
        }
        if(_board->bPawnBoard&mask){
                *p = Pawn;
                *pc = Black;
		*PieceID = 6;
                goto lblColors;
                goto lblColors;
        }
        if(_board->bKnightBoard&mask){
                *p = Knight;
                *pc = Black;
		*PieceID = 7;
                goto lblColors;
                goto lblColors;
        }
        if(_board->bKingBoard&mask){
                *p = King;
                *pc = Black;
		*PieceID = 8;
                goto lblColors;
                goto lblColors;
        }
        if(_board->bQueenBoard&mask){
                *p = Queen;
                *pc = Black;
		*PieceID = 9;
                goto lblColors;
                goto lblColors;
        }
        if(_board->bBishopBoard&mask){
                *p = Bishop;
                *pc = Black;
		*PieceID = 10;
                goto lblColors;
                goto lblColors;
        }
        if(_board->bRookBoard&mask){
                *p = Rook;
                *pc = Black;
		*PieceID = 11;
                goto lblColors;
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

void PointingAtKing(struct board *_board,char bOrW){

	BitBoard enemyBoard = bOrW ? _board->bPieces : _board->wPieces;
	BitBoard blockerBoard = bOrW ? _board->wPieces|enemyBoard : _board->bPieces|enemyBoard;
	BitBoard pos = _board->pointer;
	BitBoard emptySquare =0Ull;
	BitBoard canCapture =0Ull;
	BitBoard attackMask =  	(pos<<1&~lookupTable.HFile) |
				(pos>>1&~lookupTable.AFile) |
				(pos<<8&~lookupTable.eighthRank) |
				(pos>>8&~lookupTable.firstRank)  |
				(pos<<9&~lookupTable.eighthRank&~lookupTable.HFile) | 
				(pos>>9&~lookupTable.firstRank&~lookupTable.AFile)|
				(pos<<7&~lookupTable.eighthRank&~lookupTable.AFile) |
				(pos>>7&~lookupTable.firstRank&~lookupTable.HFile) ;

	emptySquare = attackMask&~blockerBoard;
	canCapture = attackMask&enemyBoard;
	_board->PieceCouldGo = emptySquare;
	_board->PieceCouldCapture= canCapture;
}

void PointingAtQueen(struct board *_board,char bOrW){

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

void PointingAtPawn(struct board *_board,char bOrW){ //0 is white

	BitBoard emptySquare =0Ull;
	BitBoard canCapture =0Ull;
	BitBoard enemyBoard = bOrW ? _board->bPieces : _board->wPieces;
	BitBoard blockerBoard = bOrW ? _board->wPieces|enemyBoard : _board->bPieces|enemyBoard;
	if(_board->pointer&lookupTable.secondRank&&bOrW&&!(_board->pointer>>16&blockerBoard)&&!(_board->pointer>>8&blockerBoard))	
		emptySquare = emptySquare | _board->pointer>>16;
	if(_board->pointer&lookupTable.seventhRank&&!bOrW&&!(_board->pointer<<16&blockerBoard)&&!(_board->pointer<<8&blockerBoard))
		emptySquare = emptySquare | _board->pointer<<16;
	if(!(_board->pointer>>8&blockerBoard)&&bOrW)
		emptySquare = emptySquare | _board->pointer>>8;
	if(!(_board->pointer<<8&blockerBoard)&&!bOrW)
		emptySquare = emptySquare | _board->pointer<<8;
	//captures
	if(bOrW&&(_board->pointer>>7)&enemyBoard)
		canCapture = canCapture| _board->pointer>>7;		
	if(bOrW&&(_board->pointer>>9)&enemyBoard)
		canCapture = canCapture| _board->pointer>>9;
	if(!bOrW&&(_board->pointer<<7)&enemyBoard)
		canCapture = canCapture| _board->pointer<<7;
	if(!bOrW&&(_board->pointer<<9)&enemyBoard)
		canCapture = canCapture| _board->pointer<<9;

	_board->PieceCouldGo = emptySquare;
	_board->PieceCouldCapture= canCapture;
}

BitBoard WhichCanTakeOn(struct board *_board, char pieceID, BitBoard destinationPos,char *message,char isCapture, BitBoard OriginMask){
//eg Bxe4, loop through the bishops and determine which one can take on e4k
//message: 0-cannot be completed (il*message) 1- legal, (More than one can take could be implimented int he future)
	struct board TempBoard ;
	char turns= 1;	
	int shiftIndex = 0;
	InitPieceArray(_board);
	*message = 0;
	BitBoard pieceMask = *pieceArray[pieceID];

	printf("pieceID: %d",pieceID);

	char plsDontGetStuckInALoop= 100;
	do{
		--plsDontGetStuckInALoop;
		if(!plsDontGetStuckInALoop){
			break;
		}



		int i  = shiftIndex;
		while(!((pieceMask>>i)&1)){
			++i;
			++shiftIndex;
			if(shiftIndex>64){
				break;
			}
		}
		_board->pointer = 1Ull <<shiftIndex;
		if(!(_board->pointer & OriginMask)) {//Origin is given
			goto notReturn;	
		}	

			BitBoard captureOrMove;
		if(pieceID == 1){ //white rook
			PointingAtARook(_board,1);
			captureOrMove = isCapture ? _board->PieceCouldCapture: _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
			
		}
		if(pieceID == 7){ //black rook
			PointingAtARook(_board,0);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}

		if(pieceID == 3){ //white bishop 
			PointingAtBishop(_board,1);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 9){ //black bishop 
			PointingAtBishop(_board,0);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}

		if(pieceID == 2){ //white knight 
			PointingAtAKnight(_board,1);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 8){ //black knight 
			PointingAtAKnight(_board,0);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 5){ //white queen 
			PointingAtQueen(_board,1);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 11){ //black queen 
			PointingAtQueen(_board,0);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 4){ //white king 
			PointingAtKing(_board,1);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 10){ //black king 
			PointingAtKing(_board,0);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}


		if(pieceID == 0){ //white pawn
			PointingAtPawn(_board,1);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		if(pieceID == 6){ //black pawn
			PointingAtPawn(_board,0);
			captureOrMove = isCapture ? _board->PieceCouldCapture : _board->PieceCouldGo;
			if(captureOrMove & destinationPos){
				*message = 1;
				return (_board->pointer);	
			}
		}
		notReturn:
		++shiftIndex;

	} while(pieceMask>>shiftIndex); 
//To DO: Make the messages work	
	
}
void UpdateBoard(struct board *_board){
	//loop the pointer, and determine the wAttackSquars and bAttackSquars
	//Determine the wpieces and bpieces 
	//
	_board->wPieces = _board->wPawnBoard | _board->wRookBoard|
		_board->wKnightBoard | _board->wBishopBoard | 
		_board->wKingBoard | _board->wQueenBoard;
	_board->bPieces = _board->bPawnBoard | _board->bRookBoard|
		_board->bKnightBoard | _board->bBishopBoard|
		_board->bKingBoard | _board->bQueenBoard;

}
