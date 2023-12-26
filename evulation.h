#pragma once
#include "def.h"
#include "func.h"
float eval(struct board *fboard){
	float r=0;
//	printf("white pawn count: %d\n",BitCount(fboard->wp));
//	PrintBitBoard(fboard->wp);
//	printf("black pawn count: %d\n",BitCount(fboard->bp));
//	PrintBitBoard(fboard->bp);
	// White
	r+= 100  * BitCount(fboard->wp);
	r+= 325  * BitCount(fboard->wh);
	r+= 325  * BitCount(fboard->wb);
	r+= 550  * BitCount(fboard->wr);
	r+= 1000 * BitCount(fboard->wq);
	// Black
	r-= 100  * BitCount(fboard->bp);
	r-= 325  * BitCount(fboard->bh);
	r-= 325  * BitCount(fboard->bb);
	r-= 550  * BitCount(fboard->br);
	r-= 1000 * BitCount(fboard->bq);
	
/*	if(r)ReadableBoard(fboard);
	printf("\n Value: %f\n\n\n",r);*/
	return r;
}
