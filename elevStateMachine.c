#include "elevStateMachine.h"





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
}

void stop(int floorNumber){
	printf("entering stop\n");
	elev_set_motor_direction(DIRN_STOP);
	elev_set_door_open_lamp(1);
	elevStateMachine.orderList[floorNumber].floorOrdered = 0;
	for (elev_button_type_t bType = BUTTON_CALL_UP; bType<BUTTON_COMMAND+1; bType++){
		if(floorNumber == N_FLOORS-1 && bType == 0){
				continue; //skipping up command at upper floor
			}
			else if(floorNumber == 0 && bType == 1){
				continue; //skipping down command at ground floor
			}
		elev_set_button_lamp(bType, floorNumber, 0);
	}
	checkForStart(floorNumber);
}

void start(elev_motor_direction_t direction){
	printf("entering start\n");
	elev_set_motor_direction(direction);
	elevStateMachine.direction = direction;
}
int checkIsOrdered(){
	int isOrdered = 0;
	for(int floor = 0; floor < N_FLOORS; floor++){
		isOrdered += elevStateMachine.orderList[floor].floorOrdered;
	}

	return isOrdered;
}