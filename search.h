#pragma once
#include "def.h"
#include "evulation.h"
#include "func.h"

float max(float a, float b){
	return a>b ? a : b;
}
float min(float a, float b){
	return a<b ? a : b;
}

float alphabeta(struct board *fboard, int depth, float alpha, float beta, int mainside){
	if(depth==0) return eval(fboard);

	if (mainside){
		float value= -800000;
		if(fboard->BoardPtrArraySize == 0)MoveGen(fboard);
		for(int i=0; i<fboard->BoardPtrArraySize; i++){
			value= max(value,alphabeta(fboard->BoardPtrArray[i], depth-1, alpha, beta, 0));
			if (value > beta) break;
			alpha = max(alpha, value);
		}
		if(depth==MAXDEPTH)return value;
		for(int i=0; i<fboard->BoardPtrArraySize; i++){ // every move this line need clean up for maxdepth-1
			free(fboard->BoardPtrArray[i]);
		}
		fboard->BoardPtrArraySize=0;
		return value;
	}
	else{
		float value= 800000;
		if(fboard->BoardPtrArraySize == 0)MoveGen(fboard);
		for(int i=0; i<fboard->BoardPtrArraySize; i++){
			value= max(value,alphabeta(fboard->BoardPtrArray[i], depth-1, alpha, beta, 1));
			if (value < alpha) break;
			beta = min(beta, value);
		}
		for(int i=0; i<fboard->BoardPtrArraySize; i++){
			free(fboard->BoardPtrArray[i]);
		}
		fboard->BoardPtrArraySize=0;
		return value;	
	}
	
}
