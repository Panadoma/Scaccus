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

	UpdateBoard(_board);
}

void AlgebraToPos(struct board *_board,const char move[],BitBoard *pos1,BitBoard *pos2){
	//There are 8 possible formats: e4, exd4, Re4, R8e4, R1e4, Rxe4,Raxe4, R1xe4 e8Q
	// R8e4, R1e4,Raxe4 R1xe4, exd4 
	int i =0;
	//InitPieceArray();
	char isCapture = 0;
	char pieceID = _board->turn ? 0 : 6; //0 and 6 are white and blacks pawn in pieceArr.	
	char originRankMod = 0;
	char originFileMod = 0;
	char destinationRankMod = 0;
	char destinationFileMod  = 0;
	while(move[i]!='\0'){
		switch (move[i]){

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
			case 'x':
				isCapture = 1;
				break;

			case 'a':
				destinationFileMod = 7;
				break;
			case 'b':
				destinationFileMod = 6;
				break;
			case 'c':
				destinationFileMod = 5;
				break;
			case 'd':
				destinationFileMod = 4;
				break;
			case 'e':
				destinationFileMod = 3;
				break;
			case 'f':
				destinationFileMod = 2;
				break;
			case 'g':
				destinationFileMod = 1;
				break;
			case 'h':
				destinationFileMod = 0;
				break;
			case '1':
				destinationRankMod = 7;
				break;
			case '2':
				destinationRankMod = 6;
				break;
			case '3':
				destinationRankMod = 5;
				break;
			case '4':
				destinationRankMod = 4;
				break;
			case '5':
				destinationRankMod = 3;
				break;
			case '6':
				destinationRankMod = 2;
				break;
			case '7':
				destinationRankMod = 1;
				break;
			case '8':
				destinationRankMod = 0;
				break;
		}
		++i;
	}
	//piece 
	//printf("pieceArray\n");
	//PrintBitBoard(*pieceArray[pieceID]);
	//printf("\n");
	//pos that it goes to 
	char message;
	*pos2 = 1Ull<< (destinationRankMod * 8 + destinationFileMod);
	*pos1 = WhichCanTakeOn(_board,pieceID,*pos2,&message, isCapture);


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

