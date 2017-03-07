////////////////////////////////////
//TTK4235 Tilpassede datasystemer //
//Heisprosjekt                    //
//Henrik Bjering Strand           //
//Håkon Yssen Rørstad             //
////////////////////////////////////

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

	elevStartUp();
    
    while (1) {
        //check for input from buttons
        checkButtonsForOrder();
        //standard actions at each flooor
        atFloorActions();
    }

    return 0;
}
