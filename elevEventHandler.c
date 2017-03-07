////////////////////////////////////
//TTK4235 Tilpassede datasystemer //
//Heisprosjekt                    //
//Henrik Bjering Strand           //
//Håkon Yssen Rørstad             //
////////////////////////////////////

#include "elevEventHandler.h"



void elevStartUp(){
	//start at the closest floor underneath
	initStateMachine();
	elev_set_motor_direction(DIRN_DOWN);
	
	while(!(elev_get_floor_sensor_signal() != -1)){
		//stuck here while not at a floor
	}

	elev_set_motor_direction(DIRN_STOP);
}



void checkButtonsForOrder(){
	if (elev_get_stop_signal()) {
		stopButtonPressed();
    }

	//itterating over floors
	for (int floorNumb = 0; floorNumb<N_FLOORS; floorNumb++){
		//itterate over different buttons at floor
		for(elev_button_type_t bType = BUTTON_CALL_UP; bType<BUTTON_COMMAND+1; bType++){
			//skipping up command at upper floor
			if(floorNumb == N_FLOORS-1 && bType == BUTTON_CALL_UP){
				continue; 
			}
			//skipping down command at ground floor
			else if(floorNumb == 0 && bType == BUTTON_CALL_DOWN){
				continue; 
			}
			
			if(elev_get_button_signal(bType,floorNumb)){
				elev_set_button_lamp(bType,floorNumb,1);
				setFloorOrder(floorNumb, bType);
			}
		}
	}
}






void checkForStart(floorNumber){
	while(1){
		// checking floors above or below depending on motor direction
		// + skipping current floor
		
		for(int floor = floorNumber + elevStateMachine.direction; 1 ; floor += elevStateMachine.direction){
			
			// breaking loop if above N_FLOORS or below ground floor
			if(floor == N_FLOORS || floor == -1){break;}

			// if ordered: continue in current direction
			if(elevStateMachine.orderList[floor].floorOrdered){
				
				start(elevStateMachine.direction);
				return; // return to main loop
			}
		}
	
		//checking floors in the other direction
		for(int floor = floorNumber; 1 ; floor += elevStateMachine.direction * -1 ){
			if(floor == N_FLOORS || floor == -1){break;}
			if(elevStateMachine.orderList[floor].floorOrdered){
				start(elevStateMachine.direction * -1);
				return;
			}
		}
		checkButtonsForOrder();
	}
}


void checkForStop(int floorNumber){
	int stopOrder = 0;

	//Stop if command is from the inside or in the same direction as elevator
	if(elevStateMachine.orderList[floorNumber].inside_command){
		stopOrder = 1;	
	}
	else if(elevStateMachine.orderList[floorNumber].dir_up && elevStateMachine.direction == DIRN_UP){
		stopOrder = 1;
	}
	else if(elevStateMachine.orderList[floorNumber].dir_down && elevStateMachine.direction == DIRN_DOWN){
		stopOrder = 1;
	}
	//stop if no other orders in the same direction
	else if(!checkForOrderInCurrentDir(floorNumber)){
		stopOrder = 1;
	}

	if (stopOrder){
		stop(floorNumber);
	}

}


void atFloorActions(){
	int floorNumber = elev_get_floor_sensor_signal();
	
	if(floorNumber != -1){
		elev_set_floor_indicator(floorNumber);
		elevStateMachine.lastFloor = floorNumber;
		checkForStop(floorNumber);
	}
}