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
 	char x=-1;
	char y=0;
	while(pointer){
		pointer<<=1;
		if(x==8){
			y++;
			x=0;
		}
		x++;
	}
	*i=x;
	*j=y;
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
uint8_t RayFillLeft(uint8_t input){ //eg 000101101 --> 11100000
	char index=7;
	uint8_t returnValue=0;
	while(!((input>>index)&1)&&index>=0){
		returnValue =returnValue|1<<index;
		--index;
	}
	return returnValue;
}

uint8_t RayFillRight(uint8_t input){ //eg 000101101 --> 11100000
	unsigned char index=0;
	uint8_t returnValue=0;
	while(!((input>>index)&1)&&index<9){
		returnValue =returnValue|1<<index;
		++index;
	}
	return returnValue;
}
void PointingAtARook(struct board *_board,char bOrW){
	

	//if((_board->pinnedPieces&_board->pointer)){  //ie if the piece is not pineed

		printf("\n");
		BitBoard EnemyBoard = bOrW ? _board->bPieces : _board->wPieces;
		BitBoard FriendBoard = bOrW ? _board->wPieces : _board->bPieces;
		char x,y;
		x=y=0;
		GetCoordinates(&x,&y,_board->pointer);		
		printf("x:%d y:%d\n",x,y);	
		uint8_t file =   GetFile(FriendBoard,x);
		uint8_t rank =   GetRank(FriendBoard,y);
		//uint8_t rank =  0b01010100;

		//rank
		uint8_t rightSide = rank<<(x);
		uint8_t leftSide  = rank>>(8-x);
		rightSide = rightSide>>(x);
		leftSide  = leftSide<<(8-x);	


		
		printf("RightSide is: ");
		print_binary(rightSide);
		printf("\n");
			

		printf("leftSide is: ");
		print_binary(leftSide);
		printf("\n");

		leftSide= RayFillRight(leftSide);
		rightSide=  RayFillRight(rightSide);


		printf("RightSideRayfilled is: ");
		print_binary(rightSide);
		printf("\n");
		printf("leftSideRayfilled is: ");
		print_binary(leftSide);
		printf("\n");

	//}	
}

void UpdateBoard(struct board *_board){
/*
	printf("\n");
	PrintBitBoard(emptyBoard.pointer);
	PointingAtARook(_board,1);	

	*/
	FENToInternal(&emptyBoard,"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R");	
	

	
}



