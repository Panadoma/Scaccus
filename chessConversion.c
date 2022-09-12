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
char GetPieceID(char blackOrWhite, char pieceChar){
	char pieceID = blackOrWhite ? 0 : 6; //0 and 6 are white and blacks pawn in pieceArr.	
		switch (pieceChar){

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
		return pieceID;
}
void CoordinateToBitBoard(const char move[3],BitBoard *pos){
	char destinationFileMod = 0;
	char destinationRankMod = 0; 
	switch(move[0])  {
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
	}
	switch(move[1]){
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
	*pos = 1Ull<< (destinationRankMod * 8 + destinationFileMod);

}
char BreakToElements(const char *move,char *finalSquare,char *isCapture,char *piece, char *mod){

	//Deterimes if the origin square is ambiguous	
	// format of the expressoins that have unambiguous origin square R8e4, R1e4,Raxe4 R1xe4, exd4 
	

	char files[2];
	char ranks[2];
	char rankOrfile	;
	files[1] = 69;
	ranks[1] = 69;
	char filecount =0;
	char rankcount =0;
	int i =0;
	while(move[i]!='\0'){
		switch (move[i]){

			case 'a':
				files[filecount] = 'a';
				++filecount ;
                                break;
                        case 'b':
				files[filecount] = 'b';
				++filecount ;
                                break;
                        case 'c':
				files[filecount] = 'c';
				++filecount ;
                                break;
                        case 'd':
				files[filecount] = 'd';
				++filecount ;
                                break;
                        case 'e':
				files[filecount] = 'e';
				++filecount ;
                                break;
                        case 'f':
				files[filecount] = 'f';
				++filecount ;
                                break;
                        case 'g':
				files[filecount] = 'g';
				++filecount ;
                                break;
                        case 'h':
				files[filecount] = 'h';
				++filecount ;
                                break;



			case '1':
				ranks[rankcount] = '1';
				++rankcount;
				break;
			case '2':
				ranks[rankcount] = '2';
				++rankcount;
				break;
			case '3':
				ranks[rankcount] = '3';
				++rankcount;
				break;
			case '4':
				ranks[rankcount] = '4';
				++rankcount;
				break;
			case '5':
				ranks[rankcount] = '5';
				++rankcount;
				break;
			case '6':
				ranks[rankcount] = '6';
				++rankcount;
				break;
			case '7':
				ranks[rankcount] = '7';
				++rankcount;
				break;
			case '8':
				ranks[rankcount] = '8';
				++rankcount;
				break;



			case 'R':
				*piece = 'R';
				break;
			case 'N':
				*piece = 'N';
				break;
			case 'B':
				*piece = 'B';
				break;
			case 'K':
				*piece = 'K';
				break;
			case 'Q':
				*piece = 'Q';
				break;


			case 'x':
				*isCapture = 1;
				break;


		}
	
		++i;
	}
	if(ranks[1] != 69){//a rank is given 
		rankOrfile = 1;
		*mod = ranks[0];
		finalSquare[0] = files[0];
		finalSquare[1] = ranks[1];
		
	}
	else if(files[1] != 69){ //a file is given 

		rankOrfile = 2;
		*mod = files[0];
		finalSquare[0] = files[1];
		finalSquare[1] = ranks[0];
	}
	else{ // origin ambiguous 

		rankOrfile = 0;
		finalSquare[0] = files[0];
		finalSquare[1] = ranks[0];
	}
	return rankOrfile;

}
BitBoard GetFileOrRankMask(char isAmbiguous,char mod){
	switch(isAmbiguous){
		case 0:
			return lookupTable.allBoard;
			break;
		case 1: 
			switch(mod){
				case '1':
					return lookupTable.firstRank;
					break;
				case '2':
					return lookupTable.secondRank;
					break;
				case '3':
					return lookupTable.thirdRank;
					break;
				case '4':
					return lookupTable.fourthRank;
					break;
				case '5':
					return lookupTable.fithRank;
					break;
				case '6':
					return lookupTable.sixthRank;
					break;
				case '7':
					return lookupTable.seventhRank;
					break;
				case '8':
					return lookupTable.eighthRank;
					break;
			}

		case 2: 
			switch(mod){
				case 'a':
					return lookupTable.AFile;
					break;
				case 'b':
					return lookupTable.BFile;
					break;
				case 'c':
					return lookupTable.CFile;
					break;
				case 'd':
					return lookupTable.DFile;
					break;
				case 'e':
					return lookupTable.EFile;
					break;
				case 'f':
					return lookupTable.FFile;
					break;
				case 'g':
					return lookupTable.GFile;
					break;
				case 'h':
					return lookupTable.HFile;
					break;
			}

	}

}


void AlgebraToPos(struct board *_board,const char *move,BitBoard *pos1,BitBoard *pos2){
	char isCapture=0;
	char piece;
	char pieceID ; 
	char message;
	char originChar;
	char finalSquare[3];
	char rankOrFile  =BreakToElements(move,finalSquare,&isCapture,&piece,&originChar);// Or neither
	pieceID = GetPieceID(_board->turn ,piece);	
	CoordinateToBitBoard(finalSquare,pos2);
	printf("isAmbiguous: %d\n",rankOrFile);
	printf("originchar: %c\n",originChar);
	printf("finalSquare %s\n",finalSquare);
	printf("piece: %c\n",piece);
	BitBoard OriginMask= GetFileOrRankMask(rankOrFile,originChar);
	printf("isCapture : %d\n",isCapture);
	*pos1 = WhichCanTakeOn(_board,pieceID,*pos2,&message, isCapture,OriginMask);

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

