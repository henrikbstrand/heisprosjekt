#include "elev.h"
#include "elevEventHandler.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator.\n");

    //Startup:
    // *go to defined floor
    // *initiallize state machine
	elevStartUp();

	//main-loop
    while (1) {
        //check for input from buttons
        checkButtonsForOrder();

        if(elevStateMachine.stopped){
        	//check for orders
        	checkForStart(elevStateMachine.lastFloor);

        }
        else{
        	//standard actions at each floor:
        	// *turn on floor indicators
        	// *stop if ordered
        	atFloorActions();	
        }
        
    }

    return 0;
}
