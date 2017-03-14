#include "elevStateMachine.h"


void initStateMachine(){
	//Set all orders to 0
	for(int floorNumber = 0; floorNumber < N_FLOORS; floorNumber++){
		elevStateMachine.orderList[floorNumber].dir_up = 0;
		elevStateMachine.orderList[floorNumber].dir_down = 0;
		elevStateMachine.orderList[floorNumber].inside_command = 0;
		elevStateMachine.orderList[floorNumber].floorOrdered = 0;
	}
	// set direction to down
	elevStateMachine.direction = DIRN_DOWN;
	// set last floor 
	elevStateMachine.lastFloor = elev_get_floor_sensor_signal();
	//stopped is initially true
	elevStateMachine.stopped = 1;
}




void setFloorOrder(int floorNumber, elev_button_type_t buttonType){
	switch(buttonType){
		case(BUTTON_CALL_UP):
		elevStateMachine.orderList[floorNumber].dir_up = 1;
		break;
		
		case(BUTTON_CALL_DOWN):
		elevStateMachine.orderList[floorNumber].dir_down = 1;
		break;

		case(BUTTON_COMMAND):
		elevStateMachine.orderList[floorNumber].inside_command = 1;
		break;
	}
	elevStateMachine.orderList[floorNumber].floorOrdered = 1;
}



void stop(int floorNumber){
	elev_set_motor_direction(DIRN_STOP);
	elev_set_door_open_lamp(1);
	startTimer();

	//Resets all order commands at current floor 
	elevStateMachine.orderList[floorNumber].floorOrdered = 0;
	elevStateMachine.orderList[floorNumber].inside_command = 0;
	elevStateMachine.orderList[floorNumber].dir_up = 0;
	elevStateMachine.orderList[floorNumber].dir_down = 0;
	
	//Turns light off in all buttons
	for (elev_button_type_t bType = BUTTON_CALL_UP; bType<BUTTON_COMMAND+1; bType++){
		//skipping up-command at upper floor
		if(floorNumber == N_FLOORS-1 && bType == BUTTON_CALL_UP){
				continue;
			}
		//skipping down-command at ground floor
		else if(floorNumber == 0 && bType == BUTTON_CALL_DOWN){
				continue;
			}

		elev_set_button_lamp(bType, floorNumber, 0);
	}

	//check buttons for orders while timer counts down
	while(!checkForTimeout()){
		checkButtonsForOrder();
	}
	//Close door when timer is done
	elev_set_door_open_lamp(0);

	elevStateMachine.stopped = 1;

	//returning to main-loop
}



void start(elev_motor_direction_t direction){
	elev_set_motor_direction(direction);
	
	elevStateMachine.direction = direction;

	elevStateMachine.stopped = 0;

	//returning to main loop
}



int checkForOrderInCurrentDir(int floorNumber){
	int isOrdered = 0;
	//skip current floor
	floorNumber += elevStateMachine.direction;
	
	for(int floor = floorNumber; 1 ; floor += elevStateMachine.direction){
		// breaking loop if above N_FLOORS or below ground floor
		if(floor == N_FLOORS || floor == -1){break;}
		
		isOrdered += elevStateMachine.orderList[floor].floorOrdered;
	}
	return isOrdered;
}




void stopButtonPressed(){
	elev_set_stop_lamp(1);
	elev_set_motor_direction(DIRN_STOP);
	
	//Resets all orders
	for(int floorNumber = 0; floorNumber<N_FLOORS;floorNumber++){
		elevStateMachine.orderList[floorNumber].floorOrdered = 0;
		elevStateMachine.orderList[floorNumber].inside_command = 0;
		elevStateMachine.orderList[floorNumber].dir_up = 0;
		elevStateMachine.orderList[floorNumber].dir_down = 0;
		
		//Turns off light in all buttons
		for (elev_button_type_t bType = BUTTON_CALL_UP; bType<BUTTON_COMMAND+1; bType++){
		//skipping up-command at upper floor
		if(floorNumber == N_FLOORS-1 && bType == BUTTON_CALL_UP){
				continue; 
			}
		//skipping down-command at ground floor
		else if(floorNumber == 0 && bType == BUTTON_CALL_DOWN){
				continue; 
			}
		elev_set_button_lamp(bType, floorNumber, 0);
		}
	}

	int floorNumber = elev_get_floor_sensor_signal();
	
	//Open door if elevator is at a floor
	if(floorNumber != -1){
		elev_set_door_open_lamp(1);
	}

	while(elev_get_stop_signal()){
		//Reads no orders while pressed
	}
	
	elev_set_stop_lamp(0);
	
	//If at a floor, stop as normal
	if(floorNumber != -1){
		stop(floorNumber);
	}
	else{
		//change state to stopped
		elevStateMachine.stopped = 1;
	}
	//return to checkButtonsForOrder()
}
