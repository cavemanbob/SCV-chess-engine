#include "func.h"

void StartConfig(){
	SetBigTable();
}
u64 test= 0b1000111011101;

int main(){
	StartConfig();
	
	/*
	struct board fboard;
	struct board* PtrBoard= &fboard;
	ResetBoard(PtrBoard);
	PrintBigTable();
	printf("%d",sizeof(fboard));
//	ApplyFen("1n1q1rk1/p1p3pp/1pbppp2/r3b1n1/6P1/1PNP1P1N/PBP1P1BP/R2Q1RK1 w - - 0 1", PtrBoard);
	ApplyFen(StartFen,PtrBoard);
	PrintBitBoard(PtrBoard->occupy);
	printf("\n\n");
	ReadableBoard(PtrBoard);*/

	struct board* x= CreateTreeNode();
/*	for(int i=x->BoardPtrArraySize;i>=0;i--){
		ReadableBoard(x);
	}*/
//	PrintTree(x,0);
//	printf("\n->%d",x->BoardPtrArraySize);
//	printf("\n->%d",x->BoardPtrArray[0]->BoardPtrArraySize);


	printf("\n\nsucess\n");
	return 0;
};
