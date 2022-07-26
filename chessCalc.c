#include "scaccus.h"


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

void print_binary(BitBoard a) {
    if (a > 1)
        print_binary(a / 2);
    printf("%d", a % 2);
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
		printf("%d\n",i);
	}
	return returnValue;



}


uint8_t GetRank(BitBoard _board, char rank){

	uint8_t returnValue=0Ull;
	char index =0;
	rank = 7 -rank;
	rank*=8;
	for(char i=7+rank;i>=rank;i--){
		returnValue = returnValue | ((_board>>i)&1Ull)<<index++;
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

void PointingAtARook(struct board *_board,char bOrW){

	//if((_board->pinnedPieces&_board->pointer)){  //ie if the piece is not pineed
		BitBoard EnemyBoard = bOrW ? _board->bPieces : _board->wPieces;
		BitBoard FriendBoard = bOrW ? _board->wPieces : _board->bPieces;
		char x,y;
		x=y=0;
		GetCoordinates(&x,&y,_board->pointer);		

		uint8_t reg =    0b00000000;
		uint8_t file=    0b01000100;
		uint8_t enemfile=0b00001000;
		uint8_t pointer= 0b0010000;
		

	//}	


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


void UpdateBoard(struct board *_board){



	
	
	PointingAtARook(_board,1);
	


	
}



