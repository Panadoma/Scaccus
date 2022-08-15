#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chessCalc.c"
void print_binary(BitBoard a){
    if (a > 1)
        print_binary(a / 2);
    printf("%d", a % 2);
}
//r1b1kb1r/pp2q3/2n3p1/2p1P2p/8/1P2QN2/PBP3PP/2KR1B1R b kq - 0 16
//r4rk1/p3ppbp/Pp1q1np1/3PpbB1/2B5/2N5/1PPQ1PPP/3RR1K1 w - - 4 18
void FENToInternal(struct board *_board,const char FEN[]){
	int i=0;	
	char rank=0,file=0;
	printf("%s\n",FEN);
	while(FEN[i]!='\0'){

		//printf("%c\n",FEN[i]);
		switch (FEN[i]){
			case '1':
				file+=1;
				break;
			case '2':
				file+=2;
				break;
			case '3':
				file+=3;
				break;
			case '4':
				file+=4;
				break;
			case '5':
				file+=5;
				break;
			case '6':
				file+=6;
				break;
			case '7':
				file+=7;
				break;

			case '/':
				file=0;
				rank++;
				break;
				
			case 'r':
				_board->bRookBoard = _board->bRookBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'n':
				_board->bKnightBoard = _board->bKnightBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'b':
				_board->bBishopBoard = _board->bBishopBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'k':
				_board->bKingBoard = _board->bKingBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'q':
				_board->bQueenBoard = _board->bQueenBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'p':
				_board->bPawnBoard = _board->bPawnBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'R':
				_board->wRookBoard = _board->wRookBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'N':
				_board->wKnightBoard = _board->wKnightBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'B':
				_board->wBishopBoard = _board->wBishopBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'K':
				_board->wKingBoard = _board->wKingBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'Q':
				_board->wQueenBoard = _board->wQueenBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;
			case 'P':
				_board->wPawnBoard = _board->wPawnBoard | (1ULL<<((rank*8)+(7-file)));
				file++;
				break;

		}
		
		i++;

	}

}

void AlgebraToPos(struct board *_board,const char move[],BitBoard *pos1,BitBoard *pos2,BitBoard pieceArr[]){

	int i =0;
	char pieceID = _board->turn ? 0 : 6; //0 and 6 are white and blacks pawn in pieceArr.	
	char rankMod = 0;
	char fileMod  = 0;
	while(move[i]!='\0'){
		switch (move[i]){

			case 'a':
				fileMod = 7;
				break;

			case 'b':

				fileMod = 6;
				break;
				
			case 'c':
				fileMod = 5;
				break;

			case 'd':
				fileMod = 4;
				break;

			case 'e':
				fileMod = 3;
				break;

			case 'f':
				fileMod = 2;
				break;
				
			case 'g':
				fileMod = 1;
				break;

			case 'h':
				fileMod = 0;
				break;

			case '1':
				rankMod = 7;
				break;
	
			case '2':
				rankMod = 6;
				break;

			case '3':
				rankMod = 5;
				break;
		
			case '4':
				rankMod = 4;
				break;

			case '5':
				rankMod = 3;
				break;

			case '6':
				rankMod = 2;
				break;

			case '7':
				rankMod = 1;
				break;

			case '8':
				rankMod = 0;
				break;
			case 'R':
				pieceID += 1;
				break;

			case 'N':
				pieceID += 2;
				break;

			case 'B':
				pieceID += 3;
				break;

			case 'K':
				pieceID += 4;
				break;

			case 'Q':
				pieceID += 5;
				break;
		}
		++i;
	}

}

void ReadFile(struct board *_board){
	char * num;
	FILE *fptr;

	if ((fptr = fopen("test.txt","r")) == NULL){
		printf("Error! opening file");

		// Program exits if the file pointer returns NULL.
		exit(1);
	}

	fscanf(fptr,"%s", &num);

	printf("%s", num);
	fclose(fptr); 

}

