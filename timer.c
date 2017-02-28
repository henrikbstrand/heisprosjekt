#include "timer.h"

clock_t startTime, endTime;

int checkForTimeout(){
	// checks the timer for timeout
	endTime = clock();
	printf("time: %f", (double)((endTime-startTime)/CLOCKS_PER_SEC));
	//return 1 if timer reach 3 seconds
	return ((double)((endTime-startTime)/CLOCKS_PER_SEC)>=1.0);
}

void startTimer(){
	startTime = clock();
}
