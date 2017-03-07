#include "timer.h"

clock_t startTime, endTime;

int checkForTimeout(){
	
	endTime = clock();
	
	//return 1 if timer reach 3 seconds
	return ((double)((endTime-startTime)/CLOCKS_PER_SEC)>=3.0);
}

void startTimer(){
	startTime = clock();
}
