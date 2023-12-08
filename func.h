#include <stdio.h>
#include <string.h>
#include "def.h"

int BitToBig(int num){
	return (num%8 + 21) + (( num/8) * 10); // update can be good
}
int BigToBit(int num){
	return ((num/10) * 8) + (num%10 + 21); // update can be good
}

void SetBigTable(){
	for(int i=0; i< BigTableSqNum; i++){
		BigTable[i]=65;
	}
	for(int i=0; i<BitBoardSqNum; i++){
		BigTable[BitToBig(i)]=i;
	}
}

void PrintBigTable(){
	for(int i=0; i<BigTableSqNum; i++){
		printf("%5d",BigTable[i]);
		if(i%10==9)printf("\n");
	}
}

void PrintBitBoard(u64 board){ 
	u64 k=0, l=0;
	k= ~(~k >> 1); // perfect way to get u64 0b10000...
	for(int i=0; i<BitBoardSqNum; i++){
		l= k & board;
		(l!=0) ? printf("%3d",1): printf("%3d",0);
		if(i%8==7)printf("\n");
		k= k >> 1;
	}
	printf("\n\n");
}

int BitCount(u64 bb){
	int count =0;
	for(int i=0;i<BitBoardSqNum; i++){
		if(bb & 1ULL) count++;
		bb= bb>>1;
	}
	return count;
}

void BitPop(u64 *bb); // code this when u need

void ResetBoard(struct board* resboard){
	memset(resboard, 0, sizeof(*resboard));
}

void ApplyFen(const char* fen, struct board* LocalBoard){
	int i=0;
	ResetBoard(LocalBoard);
	while(1){
		if(*fen=='/'){fen++;continue;}
		if(*fen==' ')break;
		switch(*fen){
				//Black pieces
			case 'r':
				LocalBoard->br |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'n':
				LocalBoard->bh |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'b':
				LocalBoard->bb |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'q':
				LocalBoard->bq |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'k':
				LocalBoard->bk |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;
			case 'p':
				LocalBoard->bp |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

				//White pieces
			case 'R':
				LocalBoard->wr |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'N':
				LocalBoard->wh |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'B':
				LocalBoard->wb |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'Q':
				LocalBoard->wq |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;

			case 'K':
				LocalBoard->wk |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;	
			case 'P':
				LocalBoard->wp |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				break;
			default:
				i+= *fen - '0' - 1; // -1 cuz every loop i is increasing already
				break;
		}
		fen++;
		i++;
	}
		fen++;
		if(*fen=='w') { LocalBoard->side=1;} else{ LocalBoard->side=0;}
		fen+=2;
		while(1){
			if(*fen=='-')break;
			if(*fen==' ')break;
			switch(*fen){
				case 'K':
					LocalBoard->castle |= 1ULL << 0;
					break;
				case 'Q':
					LocalBoard->castle |= 1ULL << 1;
					break;
				case 'k':
					LocalBoard->castle |=  1ULL<< 2;
					break;
				case 'q':
					LocalBoard->castle |= 1ULL << 3;
					break;
			}
			fen++;
		}
		fen++;
		if(*fen!='-'){
			int TransBit=0;
			while(*fen!=' '){
				switch(*fen){
					case 'a':
						break;
					case 'b':
						TransBit+=1;
						break;
					case 'c':
						TransBit+=2;
						break;
					case 'd':
						TransBit+=3;
						break;
					case 'e':
						TransBit+=4;
						break;
					case 'f':
						TransBit+=5;
						break;
					case 'g':
						TransBit+=6;
						break;
					case 'h':
						TransBit+=7;
						break;
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
						TransBit+= (8 - (*fen - '0')) * 8;
						break;
				}
				fen++;
			}
			LocalBoard->enpas=TransBit;
			fen++;
		}
		else{
			fen+=2;
		}
		if(*fen=='0'){
			fen+=2;
		}
		else if(*(fen+1)==' ' && *fen!='0'){
			LocalBoard->fifty= *fen - '0';
		}
		else{
			LocalBoard->fifty+= (*fen - '0') * 10;
			LocalBoard->fifty+= (*(fen+1) - '0');
			fen+=2;
		}
		// Warning!! movenum didnt pasted add maybe in future ?
		
	
}
int BitCheck(u64 bits, int cbit){
	return ((1ULL << cbit) & bits) != 0ULL ? 1 : 0;
}

void ReadableBoard(struct board* fboard){
	for(int i=8; i>0; i--){
		printf("%d  ",i);
		for(int j=8; j>0; j--){
			int square = (i-1)*8 + j - 1;
			if(BitCheck(fboard->occupy,square)){ // if square is occupied
				if(BitCheck(fboard->wp,square))printf("P  ");
				else if(BitCheck(fboard->wr,square))printf("R  ");
				else if(BitCheck(fboard->wh,square))printf("N  ");
				else if(BitCheck(fboard->wb,square))printf("B  ");
				else if(BitCheck(fboard->wq,square))printf("Q  ");
				else if(BitCheck(fboard->wk,square))printf("K  ");
				else if(BitCheck(fboard->bp,square))printf("p  ");
				else if(BitCheck(fboard->br,square))printf("r  ");
				else if(BitCheck(fboard->bh,square))printf("n  ");
				else if(BitCheck(fboard->bb,square))printf("b  ");
				else if(BitCheck(fboard->bq,square))printf("q  ");
				else if(BitCheck(fboard->bk,square))printf("k  ");
			}
			else printf(".  ");
		}
		printf("\n");
	}
	printf("\n   a  b  c  d  e  f  g  h\n");
}








