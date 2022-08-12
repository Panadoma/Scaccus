#include "chessCalc.c"
//One character is approximately 2:1 so keep it in that ratio
#define scaleFactorX 4
#define scaleFactorY 2



void RenderBoard(struct board _board);
void PrintAMiniBoard(struct board _board);
void PrintBitBoard(BitBoard _bitBoard);
void DrawMenu();
void IdentifyPiece(struct board *_board,piece *p,pieceColor *pc,char *isPointer, char x, char y);
void PrintCharacter(char *character,char *colorEscape,char i, char j);
void MakeAMove();


char *IdentifyCharacter(pieceColor currentPieceColor, piece currentPiece) ;
char *IdentifyForeColor(struct board *_board, char isPointer, char y, char x);

char flipflop=0;
char isEight=0;
void DrawMenu()
{


	int w,h;  //cloumns and rows of the teminal
	tc_get_cols_rows(&w,&h);	
	for(int i =0;i<=w;i++)
	{

		move_cursor(i,h);
		printf(" %s",TC_BG_WHT);

	}
	

		move_cursor(0,h);
		printf(" %s",TC_BG_WHT);	
		printf(" %s",cursor_off);
		

}

void RenderBoard(struct board _board){
	

		
	//Opening ritual	
	clear_screen();		
	tc_enter_alt_screen();	
	tc_echo_off();	
	clear_screen();		

	
	piece currentPiece;
	pieceColor currentPieceColor;
	char isPointer=0;
	char colorEscape[32];
	char character[16];
	
	for(char i=7; i>=0; --i){
		for(char j=0; j<8; ++j){	

			strcpy(colorEscape,IdentifyForeColor(&_board,0,i, j));
			IdentifyPiece(&_board,&currentPiece ,&currentPieceColor ,&isPointer,i, j);
			strcpy(character,IdentifyCharacter(currentPieceColor,currentPiece));
			PrintCharacter(character,colorEscape,i,j);
		}
	}
}

void PrintCharacter(char *character,char *colorEscape,char i, char j){

		int w,h;  //cloumns and rows of the teminal
		tc_get_cols_rows(&w,&h);	
		int lol = 120;		
		int lob = 64;
		int loc =23;
		if(isEight==8)	{
			flipflop=!flipflop;
			isEight=0;
		}

		isEight++;	
		if(flipflop)
		{
			lol = 109;
			lob =104;
			loc =117;
			flipflop =0;
		}
		else {
			lol =255;
			lob =205;
			loc =178;
			flipflop=1;

		}

		for(int x=-3;x<=3;x++){
			for(int y=-1;y<=1;y++){		
				move_cursor((w/2)-20-x+j*scaleFactorX,(h/2)-10-y+i*scaleFactorY);	
			//	RGBBack_Colors(lol,lob,loc);
			//	printf(" ");
			//	puts(TC_Backtonormal);
			}
		}
		move_cursor((w/2)-20+j*scaleFactorX,(h/2)-10+i*scaleFactorY);	

		//RGBBack_Colors(lol,lob,loc);
		printf("%s",colorEscape);
		printf("%s",character);
		puts(TC_Backtonormal);

}
void PrintAMiniBoard(struct board _board){

	piece currentPiece;
	pieceColor currentPieceColor;
	char isPointer=0;
	char colorEscape[32];
	char character[16];
	
	for(char i=7; i>=0; --i){
		for(char j=0; j<8; ++j){	

			strcpy(colorEscape,IdentifyForeColor(&_board,0,i, j));
			IdentifyPiece(&_board,&currentPiece ,&currentPieceColor ,&isPointer,i, j);
			strcpy(character,IdentifyCharacter(currentPieceColor,currentPiece));
				printf("%s",colorEscape);
				printf(" %s",character);
				printf(TC_Backtonormal);
		}

		printf("\n");
	}
}



char *IdentifyCharacter(pieceColor currentPieceColor, piece currentPiece) {

	char *pieceChar;
	pieceChar = malloc(16);

	if (currentPiece !=Empty) {

		switch (currentPieceColor){
				
			case Black:
				goto ItsBlack;
				break;

			case White: 

				goto ItsWhite;
				break;

		}
		
		ItsBlack:
		switch(currentPiece){
			case Pawn: 
				strcpy(pieceChar,_w_pawn_);	
				break;

			case Rook:

				strcpy(pieceChar,_w_rook_);	
				break;

			case Knight:

				strcpy(pieceChar,_w_knight_);	
				break;

			case Bishop:
				strcpy(pieceChar,_w_bishop_);	
				break;
			
			case Queen:
				strcpy(pieceChar,_w_queen_);	
				break;

			case King:
				strcpy(pieceChar,_w_king_);	
				break;
		}      


			
		ItsWhite:
		switch(currentPiece){
			case Pawn:

				strcpy(pieceChar,_w_pawn_);	
				break;

			case Rook:

				strcpy(pieceChar,_w_rook_);	
				break;

			case Knight:
				strcpy(pieceChar,_w_knight_);	
				break;

			case Bishop:
				strcpy(pieceChar,_w_bishop_);	
				break;
			
			case Queen:
				strcpy(pieceChar,_w_queen_);	
				break;

			case King:
				strcpy(pieceChar,_w_king_);	
				break;
		}


	
		}else{ 
				strcpy(pieceChar,".");	}
		return pieceChar;


}

char *IdentifyForeColor(struct board *_board, char isPointer, char y, char x){  //x and y are most likely misnamed
	
	char *returnValue;
	returnValue = malloc(32);
	char pos= 63 - (x + 8*y);
	BitBoard mask = 1ULL <<pos;
	
	if(_board->bPieces&mask){
		returnValue = bColor ;
	}

	if(_board->wPieces&mask){

		returnValue = wColor;
	}
	return returnValue;


}




void ini()
{
}

void clos()
{
	getchar();		
	clear_screen();
	tc_echo_on();
	tc_exit_alt_screen();
}



int main(void){
	FENToInternal(&emptyBoard,"r4rk1/p3ppbp/Pp1q1np1/3PpbB1/2B5/2N5/1PPQ1PPP/3RR1K1");
	emptyBoard.wPieces=emptyBoard.wPawnBoard|emptyBoard.wRookBoard|emptyBoard.wKnightBoard|emptyBoard.wBishopBoard|emptyBoard.wQueenBoard|emptyBoard.wKingBoard;
	emptyBoard.bPieces=emptyBoard.bPawnBoard|emptyBoard.bRookBoard|emptyBoard.bKnightBoard|emptyBoard.bBishopBoard|emptyBoard.bQueenBoard|emptyBoard.bKingBoard;
	RenderBoard(emptyBoard);
//	DrawMenu();
	clos();
     /*
	piece p;
	pieceColor pc;
	char isPointer[32];
	strcpy(isPointer,IdentifyForeColor(&startingBoard,0,7,0));
*/


//	PrintBitBoard(KnightAttack(startingBoard.pointer));
//	PrintBitBoard(startingBoard.PieceCouldCapture);
//	PrintBitBoard(KnightAttack(startingBoard.pointer));
	return 0;
}







