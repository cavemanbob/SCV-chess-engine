#include "func.h"

void StartConfig(){
	SetBigTable();
}
u64 test= 0b1000111011101;

int main(){
	StartConfig();
	
	
	struct board fboard;
	struct board* PtrBoard= &fboard;
	ResetBoard(PtrBoard);
	ApplyFen("1n1q1rk1/p1p3pp/1pbppp2/r3b1n1/6P1/1PNP1P1N/PBP1P1BP/R2Q1RK1 w - - 0 1", PtrBoard);
	PrintBitBoard(PtrBoard->occupy);
	printf("\n\n");
	ReadableBoard(PtrBoard);
	printf("\n\nsucess\n");
	return 0;
};
