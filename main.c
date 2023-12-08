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
	ApplyFen(StartFen, PtrBoard);
	PrintBitBoard(PtrBoard->occupy);
	printf("sucess\n");
	return 0;
};
