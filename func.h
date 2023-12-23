#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "def.h"


// Name of function cuz of A BUNCH OF LOOP BETWEEN FUNCTIONS


int BitToBig(int num);
int BigToBit(int num);
void SetBigTable();
void PrintBigTable();
void PrintBitBoard(u64 board); 
int BitCount(u64 bb);
int BitCount(u64 bb);
void BitPop(u64 *bb); // code this when u need
void ApplyFen(const char* fen, struct board* LocalBoard);
int BitCheck(u64 bits, int cbit);
void ReadableBoard(struct board* fboard);
void Nodeloop(struct board* lboard, int deep);
struct board* CreateTreeNode();
void MoveAndDo(int from, int to, char PieceType, struct board* CNodePtr);
void MoveGen(struct board* Ptrfboard);
void PrintMove(int from, int to);







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
void PrintMove(int from, int to){
	char key[8]={'a','b','c','d','e','f','g','h'};
	printf("%c%d ",key[from%8],8-from/8);
	printf("%c%d \n",key[to%8],8-to/8);
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
//	resboard->br=0ULL;	
}

void ApplyFen(const char* fen, struct board* LocalBoard){
	int i=0;
//	ResetBoard(LocalBoard); //bugg
//	memset(&LocalBoard, 0, sizeof(&LocalBoard)); // clear LocalBoard
	while(1){
//		printf("br: %llu\n",LocalBoard->br);
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
				LocalBoard->woccupy |= 1ULL << (63- i);
				break;

			case 'N':
				LocalBoard->wh |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->woccupy |= 1ULL << (63- i);
				break;

			case 'B':
				LocalBoard->wb |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->woccupy |= 1ULL << (63- i);
				break;

			case 'Q':
				LocalBoard->wq |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->woccupy |= 1ULL << (63- i);
				break;

			case 'K':
				LocalBoard->wk |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->woccupy |= 1ULL << (63- i);
				break;	
			case 'P':
				LocalBoard->wp |= 1ULL << (63- i);
				LocalBoard->occupy |= 1ULL << (63- i);
				LocalBoard->woccupy |= 1ULL << (63- i);
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
	printf("\nApplyFen Succesfully Applied\n");
	
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
void PrintTree(struct board *fboard, int deep){
	printf("%d",fboard->BoardPtrArraySize);
	if(fboard->BoardPtrArraySize!=0){
		for(int i=0; i<fboard->BoardPtrArraySize; i++){
//			ReadableBoard(fboard->BoardPtrArray[i]);
//			printf("\n%3d",i);
			PrintTree(fboard->BoardPtrArray[i], deep+1);
		}
	}
	return;
}

void Nodeloop(struct board* lboard, int deep){
//	printf("%d\n",lboard->BoardPtrArraySize);
//	printf("Node in board is %d:\n",deep);
//	ReadableBoard(lboard);
	if(deep>0){
		MoveGen(lboard);
//		printf("Compilated\n");
		for(int i=0; i<lboard->BoardPtrArraySize; i++){
			struct board inboard= *(lboard->BoardPtrArray[i]);
			ReadableBoard(&inboard);
		}
//		printf("lboard: %d\n",lboard->BoardPtrArraySize);
		for(int i=0; i<lboard->BoardPtrArraySize; i++){
			Nodeloop(lboard->BoardPtrArray[i], deep-1);
		}
	}
}

struct board* CreateTreeNode(){
	struct board* StartBoard= (struct board *)calloc(1,sizeof(struct board)); // Clean board created
//	ApplyFen("r3kb1r/p1p4p/1pn1Nnp1/3p2q1/P1P3R1/1QNPBp1p/1P2P3/R3KN1R w - - 0 1", StartBoard);
	ApplyFen(StartFen, StartBoard);
	ReadableBoard(StartBoard);  //debug line
	Nodeloop(StartBoard,2);
	return StartBoard;
}
void MoveAndDo(int from, int to, char PieceType, struct board* CNodePtr){
//	printf("MoveAndDo\n");
	// move from to
//	printf("%c From %d to %d\n",PieceType,from, to);

//	PrintMove(63-from, 63-to);		
	
	struct board* fboard = (struct board*) malloc(sizeof(struct board));
	*fboard = *CNodePtr;

	fboard->occupy ^= 1ULL << from;
	fboard->occupy |= 1ULL << to;
	if(fboard->side==1){
		fboard->woccupy ^= 1ULL << from;
		fboard->woccupy |= 1ULL << to;
		switch(PieceType){
			case 'p':
				fboard->wp ^= 1ULL << from;
				fboard->wp |= 1ULL << to;
				break;
			case 'r':
				fboard->wr ^= 1ULL << from;
				fboard->wr |= 1ULL << to;
				break;
			case 'h':
				fboard->wh ^= 1ULL << from;
				fboard->wh |= 1ULL << to;
				break;
			case 'b':
				fboard->wb ^= 1ULL << from;
				fboard->wb |= 1ULL << to;
				break;
			case 'q':
				fboard->wq ^= 1ULL << from;
				fboard->wq |= 1ULL << to;
				break;
			case 'k':
				fboard->wk ^= 1ULL << from;
				fboard->wk |= 1ULL << to;
				break;
		}		
	}
	else{
		fboard->boccupy ^= 1ULL << from;
		fboard->boccupy |= 1ULL << to;
		switch(PieceType){
			case 'p':
				fboard->bp ^= 1ULL << from;
				fboard->bp |= 1ULL << to;
				break;
			case 'r':
				fboard->br ^= 1ULL << from;
				fboard->br |= 1ULL << to;
				break;
			case 'h':
				fboard->bh ^= 1ULL << from;
				fboard->bh |= 1ULL << to;
				break;
			case 'b':
				fboard->bb ^= 1ULL << from;
				fboard->bb |= 1ULL << to;
				break;
			case 'q':
				fboard->bq ^= 1ULL << from;
				fboard->bq |= 1ULL << to;
				break;
			case 'k':
				fboard->bk ^= 1ULL << from;
				fboard->bk |= 1ULL << to;
				break;
		}	
	}
	// Add branch to array
	fboard->side = fboard->side==0ULL ? 1ULL : 0ULL; // for min max
	CNodePtr->BoardPtrArray[CNodePtr->BoardPtrArraySize]= fboard;
	CNodePtr->BoardPtrArraySize+=1;
//	ReadableBoard(PtrNewBoard);
//	printf("End of MoveAndDo\n");
}


void MoveGen(struct board* Ptrfboard){ //Its looking like true for White  but when i=61 its cruhsing after a move 
	struct board kboard= *Ptrfboard;
	struct board fboard= *Ptrfboard;
	// search move for white
	if(fboard.side == 1){
		printf("Search for white\n");
		for(int i=0; i<64; i++){
			if(fboard.woccupy & (1ULL << (63 - i))){
				// There are a white piece at 63-i
				if(fboard.wp & (1ULL << (63 - i))){
				//	printf("\n%d \n",i);
					//there are a white pawn
					if(BigTable[BitToBig(i)-11]!=65 && (fboard.boccupy & 1ULL << (9+63-i)) ) MoveAndDo(63-i,9+63-i,'p',Ptrfboard);
					if(BigTable[BitToBig(i)-9]!=65 && (fboard.boccupy & 1ULL << (7+63-i)) ) MoveAndDo(63-i,7+63-i,'p',Ptrfboard);
					if(!(fboard.occupy & 1ULL << (8+63-i))){
						MoveAndDo(63-i,8+63-i,'p',Ptrfboard);
						if(i > 47 && !(fboard.occupy & 1ULL << (16+63-i))) MoveAndDo(63-i,16+63-i,'p',Ptrfboard);
					}
				}
				else if(fboard.wr & (1ULL << (63 - i)) || fboard.wq & (1ULL << (63 - i))){
					//there are a white rook or queen
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig(i)+j*1]!=65 && !(fboard.occupy & 1ULL << (-1*j+63-i))) MoveAndDo(63-i,-1*j+63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig(i)+j*1]!=65 && (fboard.boccupy & 1ULL << (-1*j+63-i))) MoveAndDo(63-i,-1*j+63-i,'r',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))- j]!=65 && !(fboard.occupy & 1ULL << (+1*j+63-i))) MoveAndDo(63-i,j + 63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*-1]!=65 && (fboard.boccupy & 1ULL << (+1*j+63-i))) MoveAndDo(63-i,+1*j+63-i,'r',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*10]!=65 && !(fboard.occupy & 1ULL << (-8*j+63-i))) MoveAndDo(63-i,-8*j+63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*10]!=65 && (fboard.boccupy & 1ULL << (-8*j+63-i))) MoveAndDo(63-i,-8*j+63-i,'r',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*-10]!=65 && !(fboard.occupy & 1ULL << (+8*j+63-i))) MoveAndDo(63-i,+8*j+63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*-10]!=65 && (fboard.boccupy & 1ULL << (+8*j+63-i))) MoveAndDo(63-i,+8*j+63-i,'r',Ptrfboard);
							break;
						}
					}
				//end of rook
				}
				else if(fboard.wh & (1ULL << (63 - i))){
					//there are a white knight
					if(BigTable[BitToBig(i)-12]!=65 && !(fboard.woccupy & 1ULL << (10+63-i)) ) MoveAndDo(63-i,10+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)-21]!=65 && !(fboard.woccupy & 1ULL << (17+63-i)) ) MoveAndDo(63-i,17+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)-19]!=65 && !(fboard.woccupy & 1ULL << (15+63-i)) ) MoveAndDo(63-i,15+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)-8]!=65  && !(fboard.woccupy & 1ULL << (6+63-i)) ) MoveAndDo(63-i,6+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+12]!=65 && !(fboard.woccupy & 1ULL << (-10+63-i)) ) MoveAndDo(63-i,-10+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+21]!=65 && !(fboard.woccupy & 1ULL << (-17+63-i)) ) MoveAndDo(63-i,-17+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+19]!=65 && !(fboard.woccupy & 1ULL << (-15+63-i)) ) MoveAndDo(63-i,-15+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+8]!=65  && !(fboard.woccupy & 1ULL << (-6+63-i)) ) MoveAndDo(63-i,-6+63-i,'h',Ptrfboard);
				}
				if(fboard.wb & (1ULL << (63 - i)) || fboard.wq & (1ULL << (63 - i))){ // there is a if not else if cuz to check queen (queen= rook + bishop)
					//there are a white bishop	
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*1-10*j]!=65 && !(fboard.occupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(63-i,-1*j+8*j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*1-10*j]!=65 && (fboard.boccupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(63-i,-1*j+8*j+63-i,'b',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && !(fboard.occupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(63-i,+1*j+8*j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && (fboard.boccupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(63-i,+1*j+8*j+63-i,'b',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*10-j]!=65 && !(fboard.occupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(63-i,-8*j+j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*10-j]!=65 && (fboard.boccupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(63-i,-8*j+j+63-i,'b',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*10+j]!=65 && !(fboard.occupy & 1ULL << (-8*j-j+63-i))) MoveAndDo(63-i,-8*j-j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*10+j]!=65 && (fboard.boccupy & 1ULL << (-8*j-j+63-i))) MoveAndDo(63-i,-8*j-j+63-i,'b',Ptrfboard);
							break;
						}
					}
					//end of bishop
				}
				else if(fboard.wk & (1ULL << (63 - i))){
					//there are a white king
					if(BigTable[BitToBig(i)-11]!=65 && !(fboard.woccupy & 1ULL << (9+63-i))) MoveAndDo( 63-i, 9+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)-10]!=65 && !(fboard.woccupy & 1ULL << (8+63-i))) MoveAndDo( 63-i, 8+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)-9]!=65  && !(fboard.woccupy & 1ULL << (7+63-i))) MoveAndDo( 63-i, 7+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)-1]!=65  && !(fboard.woccupy & 1ULL << (1+63-i))) MoveAndDo( 63-i, 1+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+1]!=65  && !(fboard.woccupy & 1ULL << (-1+63-i))) MoveAndDo( 63-i, -1+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+11]!=65 && !(fboard.woccupy & 1ULL << (-9+63-i))) MoveAndDo( 63-i, -9+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+10]!=65 && !(fboard.woccupy & 1ULL << (-8+63-i))) MoveAndDo( 63-i, -8+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+9]!=65  && !(fboard.woccupy & 1ULL << (-7+63-i))) MoveAndDo( 63-i, -7+63-i,'k',Ptrfboard);
				}
			}
		}
/*		printf("Castle Time\n");
		if(fboard.castle & 1ULL << 0 && !(fboard.occupy & 1ULL << 2 ) && !(fboard.occupy & 1ULL << 1)){ // white king side castle
			struct board Kboard=fboard;
			Kboard.castle &= 1ULL << 0;
			Kboard.wr ^= 1ULL;
			Kboard.occupy ^= 1ULL;
			Kboard.woccupy ^= 1ULL;
			Kboard.wr ^= 1ULL << 2;
			Kboard.occupy ^= 1ULL << 2;
			Kboard.woccupy ^= 1ULL << 2;
			MoveAndDo(Kboard, 3, 1,'k',Ptrfboard);
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
			MoveAndDo(Qboard, 3, 5,'k',Ptrfboard);
		}
		*/
		//end of white side
	}
	// search move for black
	else{
		printf("Search for black\n");
		for(int i=0; i<64; i++){
			if(fboard.boccupy & (1ULL << (63 - i))){
				// There are a white piece at 63-i
				if(fboard.bp & (1ULL << (63 - i))){
				//	printf("\n%d \n",i);
					//there are a white pawn
					if(BigTable[BitToBig(i)+11]!=65 && (fboard.woccupy & 1ULL << (-9+63-i)) ) MoveAndDo(63-i,-9+63-i,'p',Ptrfboard);
					if(BigTable[BitToBig(i)+9]!=65 && (fboard.woccupy & 1ULL << (-7+63-i)) ) MoveAndDo(63-i,-7+63-i,'p',Ptrfboard);
					if(!(fboard.occupy & 1ULL << (-8+63-i))){
						MoveAndDo(63-i,-8+63-i,'p',Ptrfboard);
						if(i < 16 && !(fboard.occupy & 1ULL << (-16+63-i))) MoveAndDo(63-i,-16+63-i,'p',Ptrfboard);
					}
				}
				else if(fboard.br & (1ULL << (63 - i)) || fboard.bq & (1ULL << (63 - i))){
					//there are a white rook or queen
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig(i)+j*1]!=65 && !(fboard.occupy & 1ULL << (-1*j+63-i))) MoveAndDo(63-i,-1*j+63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig(i)+j*1]!=65 && (fboard.woccupy & 1ULL << (-1*j+63-i))) MoveAndDo(63-i,-1*j+63-i,'r',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))- j]!=65 && !(fboard.occupy & 1ULL << (+1*j+63-i))) MoveAndDo(63-i,j + 63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*-1]!=65 && (fboard.woccupy & 1ULL << (+1*j+63-i))) MoveAndDo(63-i,+1*j+63-i,'r',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*10]!=65 && !(fboard.occupy & 1ULL << (-8*j+63-i))) MoveAndDo(63-i,-8*j+63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*10]!=65 && (fboard.woccupy & 1ULL << (-8*j+63-i))) MoveAndDo(63-i,-8*j+63-i,'r',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*-10]!=65 && !(fboard.occupy & 1ULL << (+8*j+63-i))) MoveAndDo(63-i,+8*j+63-i,'r',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*-10]!=65 && (fboard.woccupy & 1ULL << (+8*j+63-i))) MoveAndDo(63-i,+8*j+63-i,'r',Ptrfboard);
							break;
						}
					}
				//end of rook
				}
				else if(fboard.bh & (1ULL << (63 - i))){
					//there are a white knight
					if(BigTable[BitToBig(i)-12]!=65 && !(fboard.boccupy & 1ULL << (10+63-i)) ) MoveAndDo(63-i,10+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)-21]!=65 && !(fboard.boccupy & 1ULL << (17+63-i)) ) MoveAndDo(63-i,17+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)-19]!=65 && !(fboard.boccupy & 1ULL << (15+63-i)) ) MoveAndDo(63-i,15+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)-8]!=65  && !(fboard.boccupy & 1ULL << (6+63-i)) ) MoveAndDo(63-i,6+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+12]!=65 && !(fboard.boccupy & 1ULL << (-10+63-i)) ) MoveAndDo(63-i,-10+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+21]!=65 && !(fboard.boccupy & 1ULL << (-17+63-i)) ) MoveAndDo(63-i,-17+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+19]!=65 && !(fboard.boccupy & 1ULL << (-15+63-i)) ) MoveAndDo(63-i,-15+63-i,'h',Ptrfboard);
					if(BigTable[BitToBig(i)+8]!=65  && !(fboard.boccupy & 1ULL << (-6+63-i)) ) MoveAndDo(63-i,-6+63-i,'h',Ptrfboard);
				}
				if(fboard.bb & (1ULL << (63 - i)) || fboard.bq & (1ULL << (63 - i))){ // there is a if not else if cuz to check queen (queen= rook + bishop)
					//there are a white bishop	
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*1-10*j]!=65 && !(fboard.occupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(63-i,-1*j+8*j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*1-10*j]!=65 && (fboard.woccupy & 1ULL << (-1*j+8*j+63-i))) MoveAndDo(63-i,-1*j+8*j+63-i,'b',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && !(fboard.occupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(63-i,+1*j+8*j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*-1-10*j]!=65 && (fboard.woccupy & 1ULL << (+1*j+8*j+63-i))) MoveAndDo(63-i,+1*j+8*j+63-i,'b',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*10-j]!=65 && !(fboard.occupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(63-i,-8*j+j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*10-j]!=65 && (fboard.woccupy & 1ULL << (-8*j+j+63-i))) MoveAndDo(63-i,-8*j+j+63-i,'b',Ptrfboard);
							break;
						}
					}
					for(int j=1; j<8;j++){
						if(BigTable[BitToBig((i))+j*10+j]!=65 && !(fboard.occupy & 1ULL << (-8*j-j+63-i))) MoveAndDo(63-i,-8*j-j+63-i,'b',Ptrfboard);
						else{	
							if(BigTable[BitToBig((i))+j*10+j]!=65 && (fboard.woccupy & 1ULL << (-8*j-j+63-i))) MoveAndDo(63-i,-8*j-j+63-i,'b',Ptrfboard);
							break;
						}
					}
					//end of bishop
				}
				else if(fboard.bk & (1ULL << (63 - i))){
					//there are a white king
					if(BigTable[BitToBig(i)-11]!=65 && !(fboard.boccupy & 1ULL << (9+63-i))) MoveAndDo( 63-i, 9+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)-10]!=65 && !(fboard.boccupy & 1ULL << (8+63-i))) MoveAndDo( 63-i, 8+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)-9]!=65  && !(fboard.boccupy & 1ULL << (7+63-i))) MoveAndDo( 63-i, 7+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)-1]!=65  && !(fboard.boccupy & 1ULL << (1+63-i))) MoveAndDo( 63-i, 1+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+1]!=65  && !(fboard.boccupy & 1ULL << (-1+63-i))) MoveAndDo( 63-i, -1+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+11]!=65 && !(fboard.boccupy & 1ULL << (-9+63-i))) MoveAndDo( 63-i, -9+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+10]!=65 && !(fboard.boccupy & 1ULL << (-8+63-i))) MoveAndDo( 63-i, -8+63-i,'k',Ptrfboard);
					if(BigTable[BitToBig(i)+9]!=65  && !(fboard.boccupy & 1ULL << (-7+63-i))) MoveAndDo( 63-i, -7+63-i,'k',Ptrfboard);
				}
			}
		}
/*		printf("Castle Time\n");
		if(fboard.castle & 1ULL << 0 && !(fboard.occupy & 1ULL << 2 ) && !(fboard.occupy & 1ULL << 1)){ // white king side castle
			struct board Kboard=fboard;
			Kboard.castle &= 1ULL << 0;
			Kboard.wr ^= 1ULL;
			Kboard.occupy ^= 1ULL;
			Kboard.woccupy ^= 1ULL;
			Kboard.wr ^= 1ULL << 2;
			Kboard.occupy ^= 1ULL << 2;
			Kboard.woccupy ^= 1ULL << 2;
			MoveAndDo(Kboard, 3, 1,'k',Ptrfboard);
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
			MoveAndDo(Qboard, 3, 5,'k',Ptrfboard);
		}
		*/
		//end of white side
	}
}






