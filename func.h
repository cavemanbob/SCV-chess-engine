#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	u64 k=1, l=0;
//	k= ~(~k >> 1); // perfect way to get u64 0b10000...
	k= k<<63; // 100..ULL another way 
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
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'n':
				LocalBoard->bh |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'b':
				LocalBoard->bb |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'q':
				LocalBoard->bq |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'k':
				LocalBoard->bk |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;
			case 'p':
				LocalBoard->bp |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

				//White pieces
			case 'R':
				LocalBoard->wr |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'N':
				LocalBoard->wh |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'B':
				LocalBoard->wb |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'Q':
				LocalBoard->wq |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;

			case 'K':
				LocalBoard->wk |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
				break;	
			case 'P':
				LocalBoard->wp |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->boccupy |= 1ULL << (63- i);
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
void CreateNewNode();
void MoveAndDo(struct board fboard, int from, int to, char PieceType){
	// move from to
	fboard.occupy ^= 1ULL << from;
	fboard.occupy |= 1ULL << to;
	if(fboard.side==1){
		fboard.woccupy ^= 1ULL << from;
		fboard.woccupy |= 1ULL << to;
		switch(PieceType){
			case 'p':
				fboard.wp ^= 1ULL << from;
				fboard.wp |= 1ULL << to;
				break;
			case 'r':
				fboard.wr ^= 1ULL << from;
				fboard.wr |= 1ULL << to;
				break;
			case 'h':
				fboard.wh ^= 1ULL << from;
				fboard.wh |= 1ULL << to;
				break;
			case 'b':
				fboard.wb ^= 1ULL << from;
				fboard.wb |= 1ULL << to;
				break;
			case 'q':
				fboard.wq ^= 1ULL << from;
				fboard.wq |= 1ULL << to;
				break;
			case 'k':
				fboard.wk ^= 1ULL << from;
				fboard.wk |= 1ULL << to;
				break;
		}		
	}
	else{
		fboard.boccupy ^= 1ULL << from;
		fboard.boccupy |= 1ULL << to;
		switch(PieceType){
			case 'p':
				fboard.bp ^= 1ULL << from;
				fboard.bp |= 1ULL << to;
				break;
			case 'r':
				fboard.br ^= 1ULL << from;
				fboard.br |= 1ULL << to;
				break;
			case 'h':
				fboard.bh ^= 1ULL << from;
				fboard.bh |= 1ULL << to;
				break;
			case 'b':
				fboard.bb ^= 1ULL << from;
				fboard.bb |= 1ULL << to;
				break;
			case 'q':
				fboard.bq ^= 1ULL << from;
				fboard.bq |= 1ULL << to;
				break;
			case 'k':
				fboard.bk ^= 1ULL << from;
				fboard.bk |= 1ULL << to;
				break;
		}		
	}
	// Add branch to array
	struct board* PtrNBoard= (struct board*) malloc(sizeof(struct board));

}


void MoveGen(struct board fboard){
	// search move for white
	if(fboard.side == 1){
		for(int i=0; i<64; i++){
			if(fboard.woccupy & (1ULL << (63 - i))){
				// There are a white piece at 63-i
				if(fboard.wp & (1ULL << (63 - i))){
					//there are a white pawn
					if(BigTable[BitToBig(i)-11]!=65 && (fboard.boccupy & 1ULL << (9+63-i)) ) MoveAndDo(fboard,63-i,9+63-i,'p');
					if(BigTable[BitToBig(i)-9]!=65 && (fboard.boccupy & 1ULL << (7+63-i)) ) MoveAndDo(fboard,63-i,7+63-i,'p');
					if(fboard.occupy & 1ULL << (8+63-i)) MoveAndDo(fboard,63-i,8+63-i,'p');
					if(i < 17 && fboard.occupy & 1ULL << (16+63-i)) MoveAndDo(fboard,63-i,16+63-i,'p');
				}
				else if(fboard.wr & (1ULL << (63 - i)) || fboard.wq & (1ULL << (63 - i))){
					//there are a white rook or queen
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*1]!=65 && (fboard.occupy & 1ULL << (-1*j+63-i))) MoveAndDo(fboard,63-i,-1*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*1]!=65 && (fboard.boccupy & 1ULL << (-1*j+63-i))) MoveAndDo(fboard,63-i,-1*j+63-i,'r');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-1]!=65 && (fboard.occupy & 1ULL << (+1*j+63-i))) MoveAndDo(fboard,63-i,+1*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*-1]!=65 && (fboard.boccupy & 1ULL << (+1*j+63-i))) MoveAndDo(fboard,63-i,+1*j+63-i,'r');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*10]!=65 && (fboard.occupy & 1ULL << (-8*j+63-i))) MoveAndDo(fboard,63-i,-8*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*10]!=65 && (fboard.boccupy & 1ULL << (-8*j+63-i))) MoveAndDo(fboard,63-i,-8*j+63-i,'r');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-10]!=65 && (fboard.occupy & 1ULL << (+8*j+63-i))) MoveAndDo(fboard,63-i,+8*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*-10]!=65 && (fboard.boccupy & 1ULL << (+8*j+63-i))) MoveAndDo(fboard,63-i,+8*j+63-i,'r');
							break;
						}
					}
				//end of rook
				}
				else if(fboard.wh & (1ULL << (63 - i))){
					//there are a white knight
					if(BigTable[BitToBig(i)-12]!=65 && !(fboard.woccupy & 1ULL << (10+63-i)) ) MoveAndDo(fboard,63-i,10+63-i,'h');
					if(BigTable[BitToBig(i)-21]!=65 && !(fboard.woccupy & 1ULL << (17+63-i)) ) MoveAndDo(fboard,63-i,17+63-i,'h');
					if(BigTable[BitToBig(i)-19]!=65 && !(fboard.woccupy & 1ULL << (15+63-i)) ) MoveAndDo(fboard,63-i,15+63-i,'h');
					if(BigTable[BitToBig(i)-8]!=65  && !(fboard.woccupy & 1ULL << (6+63-i)) ) MoveAndDo(fboard,63-i,6+63-i,'h');
					if(BigTable[BitToBig(i)+12]!=65 && !(fboard.woccupy & 1ULL << (-10+63-i)) ) MoveAndDo(fboard,63-i,-10+63-i,'h');
					if(BigTable[BitToBig(i)+21]!=65 && !(fboard.woccupy & 1ULL << (-17+63-i)) ) MoveAndDo(fboard,63-i,-17+63-i,'h');
					if(BigTable[BitToBig(i)+19]!=65 && !(fboard.woccupy & 1ULL << (-15+63-i)) ) MoveAndDo(fboard,63-i,-15+63-i,'h');
					if(BigTable[BitToBig(i)+8]!=65  && !(fboard.woccupy & 1ULL << (-6+63-i)) ) MoveAndDo(fboard,63-i,-6+63-i,'h');
				}
				if(fboard.wb & (1ULL << (63 - i)) || fboard.wq & (1ULL << (63 - i))){ // there is a if not else if cuz to check queen (queen= rook + bishop)
					//there are a white bishop	
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*1-10*j]!=65 && (fboard.occupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(fboard,63-i,-1*j+8*j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*1-10*j]!=65 && (fboard.boccupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(fboard,63-i,-1*j+8*j+63-i,'b');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && (fboard.occupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(fboard,63-i,+1*j+8*j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && (fboard.boccupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(fboard,63-i,+1*j+8*j+63-i,'b');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*10-j]!=65 && (fboard.occupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(fboard,63-i,-8*j+j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*10-j]!=65 && (fboard.boccupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(fboard,63-i,-8*j+j+63-i,'b');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-10+j]!=65 && (fboard.occupy & 1ULL << (+8*j-j+63-i))) MoveAndDo(fboard,63-i,+8*j-j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*-10+j]!=65 && (fboard.boccupy & 1ULL << (+8*j-j+63-i))) MoveAndDo(fboard,63-i,+8*j-j+63-i,'b');
							break;
						}
					}
					//end of bishop
				}
				else if(fboard.wk & (1ULL << (63 - i))){
					//there are a white king
					if(BigTable[BitToBig(i)-11]!=65 && !(fboard.woccupy & 1ULL << (9+63-i))) MoveAndDo(fboard, 63-i, 9+63-i,'k');
					if(BigTable[BitToBig(i)-10]!=65 && !(fboard.woccupy & 1ULL << (8+63-i))) MoveAndDo(fboard, 63-i, 8+63-i,'k');
					if(BigTable[BitToBig(i)-9]!=65  && !(fboard.woccupy & 1ULL << (7+63-i))) MoveAndDo(fboard, 63-i, 7+63-i,'k');
					if(BigTable[BitToBig(i)-1]!=65  && !(fboard.woccupy & 1ULL << (1+63-i))) MoveAndDo(fboard, 63-i, 1+63-i,'k');
					if(BigTable[BitToBig(i)+1]!=65  && !(fboard.woccupy & 1ULL << (-1+63-i))) MoveAndDo(fboard, 63-i, -1+63-i,'k');
					if(BigTable[BitToBig(i)+11]!=65 && !(fboard.woccupy & 1ULL << (-9+63-i))) MoveAndDo(fboard, 63-i, -9+63-i,'k');
					if(BigTable[BitToBig(i)+10]!=65 && !(fboard.woccupy & 1ULL << (-8+63-i))) MoveAndDo(fboard, 63-i, -8+63-i,'k');
					if(BigTable[BitToBig(i)+9]!=65  && !(fboard.woccupy & 1ULL << (-7+63-i))) MoveAndDo(fboard, 63-i, -7+63-i,'k');
				}
			}
		}
		if(fboard.castle & 1ULL << 0 && !(fboard.occupy & 1ULL << 2 ) && !(fboard.occupy & 1ULL << 1)){ // white king side castle
			struct board Kboard=fboard;
			Kboard.castle &= 1ULL << 0;
			Kboard.wr ^= 1ULL;
			Kboard.occupy ^= 1ULL;
			Kboard.woccupy ^= 1ULL;
			Kboard.wr ^= 1ULL << 2;
			Kboard.occupy ^= 1ULL << 2;
			Kboard.woccupy ^= 1ULL << 2;
			MoveAndDo(Kboard, 3, 1,'k');
		}
		if(fboard.castle & 1ULL << 1 && !(fboard.occupy & 1ULL << 6 ) && !(fboard.occupy & 1ULL << 5) && !(fboard.occupy & 1ULL << 4)){ // white queen side castle
			struct board Qboard=fboard;
			Qboard.castle &= 1ULL << 1;
			Qboard.wr ^= 1ULL << 7;
			Qboard.occupy ^= 1ULL << 7;
			Qboard.woccupy ^= 1ULL << 7;
			Qboard.wr ^= 1ULL << 4;
			Qboard.occupy ^= 1ULL << 4;
			Qboard.woccupy ^= 1ULL << 4;
			MoveAndDo(Qboard, 3, 5,'k');
		}
		
		//end of white side
	}
	// search move for black
	else{
		for(int i=0; i<64; i++){
			if(fboard.boccupy & (1ULL << (63 - i))){
				// There are a black piece at 63-i 	
				if(fboard.bp & (1ULL << (63 - i))){
					//there are a black pawn
					if(BigTable[BitToBig(i)+11]!=65 && (fboard.woccupy & 1ULL << (-9+63-i)) ) MoveAndDo(fboard,63-i,-9+63-i,'p');
					if(BigTable[BitToBig(i)+9]!=65 && (fboard.woccupy & 1ULL << (-7+63-i)) ) MoveAndDo(fboard,63-i,-7+63-i,'p');
					if(fboard.occupy & 1ULL << (-8+63-i)) MoveAndDo(fboard,63-i,-8+63-i,'p');
					if(i > 55 && fboard.occupy & 1ULL << (-16+63-i)) MoveAndDo(fboard,63-i,-16+63-i,'p');
				}
				else if(fboard.br & (1ULL << (63 - i)) || fboard.bq & (1ULL << (63 - i))){
					//there are a black rook or queen
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*1]!=65 && (fboard.occupy & 1ULL << (-1*j+63-i))) MoveAndDo(fboard,63-i,-1*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*1]!=65 && (fboard.woccupy & 1ULL << (-1*j+63-i))) MoveAndDo(fboard,63-i,-1*j+63-i,'r');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-1]!=65 && (fboard.occupy & 1ULL << (+1*j+63-i))) MoveAndDo(fboard,63-i,+1*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*-1]!=65 && (fboard.woccupy & 1ULL << (+1*j+63-i))) MoveAndDo(fboard,63-i,+1*j+63-i,'r');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*10]!=65 && (fboard.occupy & 1ULL << (-8*j+63-i))) MoveAndDo(fboard,63-i,-8*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*10]!=65 && (fboard.woccupy & 1ULL << (-8*j+63-i))) MoveAndDo(fboard,63-i,-8*j+63-i,'r');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-10]!=65 && (fboard.occupy & 1ULL << (+8*j+63-i))) MoveAndDo(fboard,63-i,+8*j+63-i,'r');
						else{	
							if(BigTable[BitToBig((i))+j*-10]!=65 && (fboard.woccupy & 1ULL << (+8*j+63-i))) MoveAndDo(fboard,63-i,+8*j+63-i,'r');
							break;
						}
					}
				//end of rook
				}
				else if(fboard.wh & (1ULL << (63 - i))){
					//there are a black knight
					if(BigTable[BitToBig(i)-12]!=65 && !(fboard.boccupy & 1ULL << (10+63-i)) ) MoveAndDo(fboard,63-i,10+63-i,'h');
					if(BigTable[BitToBig(i)-21]!=65 && !(fboard.boccupy & 1ULL << (17+63-i)) ) MoveAndDo(fboard,63-i,17+63-i,'h');
					if(BigTable[BitToBig(i)-19]!=65 && !(fboard.boccupy & 1ULL << (15+63-i)) ) MoveAndDo(fboard,63-i,15+63-i,'h');
					if(BigTable[BitToBig(i)-8]!=65  && !(fboard.boccupy & 1ULL << (6+63-i)) ) MoveAndDo(fboard,63-i,6+63-i,'h');
					if(BigTable[BitToBig(i)+12]!=65 && !(fboard.boccupy & 1ULL << (-10+63-i)) ) MoveAndDo(fboard,63-i,-10+63-i,'h');
					if(BigTable[BitToBig(i)+21]!=65 && !(fboard.boccupy & 1ULL << (-17+63-i)) ) MoveAndDo(fboard,63-i,-17+63-i,'h');
					if(BigTable[BitToBig(i)+19]!=65 && !(fboard.boccupy & 1ULL << (-15+63-i)) ) MoveAndDo(fboard,63-i,-15+63-i,'h');
					if(BigTable[BitToBig(i)+8]!=65  && !(fboard.boccupy & 1ULL << (-6+63-i)) ) MoveAndDo(fboard,63-i,-6+63-i,'h');
				}
				if(fboard.bb & (1ULL << (63 - i)) || fboard.bq & (1ULL << (63 - i))){ // there is a if not else if cuz to check queen (queen= rook + bishop)
					//there are a black bishop	
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*1-10*j]!=65 && (fboard.occupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(fboard,63-i,-1*j+8*j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*1-10*j]!=65 && (fboard.woccupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(fboard,63-i,-1*j+8*j+63-i,'b');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && (fboard.occupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(fboard,63-i,+1*j+8*j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && (fboard.woccupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(fboard,63-i,+1*j+8*j+63-i,'b');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*10-j]!=65 && (fboard.occupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(fboard,63-i,-8*j+j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*10-j]!=65 && (fboard.woccupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(fboard,63-i,-8*j+j+63-i,'b');
							break;
						}
					}
					for(int j=0; j<8;j++){
						if(BigTable[BitToBig((i))+j*-10+j]!=65 && (fboard.occupy & 1ULL << (+8*j-j+63-i))) MoveAndDo(fboard,63-i,+8*j-j+63-i,'b');
						else{	
							if(BigTable[BitToBig((i))+j*-10+j]!=65 && (fboard.woccupy & 1ULL << (+8*j-j+63-i))) MoveAndDo(fboard,63-i,+8*j-j+63-i,'b');
							break;
						}
					}
					//end of bishop
				}
				else if(fboard.bk & (1ULL << (63 - i))){
					//there are a black king
					if(BigTable[BitToBig(i)-11]!=65 && !(fboard.boccupy & 1ULL << (9+63-i))) MoveAndDo(fboard, 63-i, 9+63-i,'k');
					if(BigTable[BitToBig(i)-10]!=65 && !(fboard.boccupy & 1ULL << (8+63-i))) MoveAndDo(fboard, 63-i, 8+63-i,'k');
					if(BigTable[BitToBig(i)-9]!=65  && !(fboard.boccupy & 1ULL << (7+63-i))) MoveAndDo(fboard, 63-i, 7+63-i,'k');
					if(BigTable[BitToBig(i)-1]!=65  && !(fboard.boccupy & 1ULL << (1+63-i))) MoveAndDo(fboard, 63-i, 1+63-i,'k');
					if(BigTable[BitToBig(i)+1]!=65  && !(fboard.boccupy & 1ULL << (-1+63-i))) MoveAndDo(fboard, 63-i, -1+63-i,'k');
					if(BigTable[BitToBig(i)+11]!=65 && !(fboard.boccupy & 1ULL << (-9+63-i))) MoveAndDo(fboard, 63-i, -9+63-i,'k');
					if(BigTable[BitToBig(i)+10]!=65 && !(fboard.boccupy & 1ULL << (-8+63-i))) MoveAndDo(fboard, 63-i, -8+63-i,'k');
					if(BigTable[BitToBig(i)+9]!=65  && !(fboard.boccupy & 1ULL << (-7+63-i))) MoveAndDo(fboard, 63-i, -7+63-i,'k');
				}
			}	
		if(fboard.castle & 1ULL << 2 && !(fboard.occupy & 1ULL << 57 ) && !(fboard.occupy & 1ULL << 58)){ // white king side castle
			struct board Kboard=fboard;
			Kboard.castle &= 1ULL << 2;
			Kboard.wr ^= 1ULL << 56;
			Kboard.occupy ^= 1ULL << 56;
			Kboard.woccupy ^= 1ULL << 56;
			Kboard.wr ^= 1ULL << 58;
			Kboard.occupy ^= 1ULL << 58;
			Kboard.woccupy ^= 1ULL << 58;
			MoveAndDo(Kboard, 59, 57,'k');
		}
		if(fboard.castle & 1ULL << 3 && !(fboard.occupy & 1ULL << 60 ) && !(fboard.occupy & 1ULL << 61) && !(fboard.occupy & 1ULL << 62)){ // white queen side castle
			struct board Qboard=fboard;
			Qboard.castle &= 1ULL << 3;
			Qboard.wr ^= 1ULL << 63;
			Qboard.occupy ^= 1ULL << 63;
			Qboard.woccupy ^= 1ULL << 63;
			Qboard.wr ^= 1ULL << 60;
			Qboard.occupy ^= 1ULL << 60;
			Qboard.woccupy ^= 1ULL << 60;
			MoveAndDo(Qboard, 59, 61,'k');
		}


		//End of black side
		}

	}
}






