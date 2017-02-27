#include "elevEventHandler.h"


void checkButtonsForOrder(){
	if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            assert(0);
        }
	//itterating over floors
	for (int floorNumb = 0; floorNumb<N_FLOORS; floorNumb++){
		//itterate over types
		for(elev_button_type_t bType = BUTTON_CALL_UP; bType<BUTTON_COMMAND+1; bType++){
			if(floorNumb == N_FLOORS-1 && bType == 0){
				continue; //skipping up command at upper floor
			}
			else if(floorNumb == 0 && bType == 1){
				continue; //skipping down command at ground floor
			}

			if(elev_get_button_signal(bType,floorNumb)){
				elev_set_button_lamp(bType,floorNumb,1);
				setFloorOrder(floorNumb, bType);
			}
		}
	}
}

void elevStartUp(){
	//start at ground floor
	initStateMachine();
	elev_set_motor_direction(DIRN_DOWN);
	while(!(elev_get_floor_sensor_signal() == 0)){
	}
	elev_set_motor_direction(DIRN_STOP);
}




void checkForStart(floorNumber){
	printf("entering checkForStart\n");


	while(1){
		printf("stuck in checkForStart!\n");
		// checking floors above or below depending on motor direction
		for(int floor = floorNumber; 1 ; floor+= elevStateMachine.direction){
			printf("got to floor: %i\n",floor);
			printf("DIR is: %i\n",elevStateMachine.direction);
			// breaking loop if above N_FLOORS or below ground floor
			if(floor == N_FLOORS || floor == -1){break;}
			// if ordered: continue in current direction
			if(elevStateMachine.orderList[floor].floorOrdered){
				printf("\n gets to start call\n\n");
				start(elevStateMachine.direction);
				return; // return to main loop
			}
		}
	
		//checking floors in the other direction
		for(int floor = floorNumber; 1 ; floor+= elevStateMachine.direction * -1){
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
	printf("entering checkForStop\n");
	int stopOrder = 0;
	if(elevStateMachine.orderList[floorNumber].inside_command){
		stopOrder = 1;
		elevStateMachine.orderList[floorNumber].inside_command = 0;
		elevStateMachine.orderList[floorNumber].dir_up = 0;
		elevStateMachine.orderList[floorNumber].dir_down = 0;
	}
	else if(elevStateMachine.orderList[floorNumber].dir_up && elevStateMachine.direction == DIRN_UP){
		stopOrder = 1;
		elevStateMachine.orderList[floorNumber].dir_up = 0;
	}
	else if(elevStateMachine.orderList[floorNumber].dir_down && elevStateMachine.direction == DIRN_DOWN){
		stopOrder = 1;
		elevStateMachine.orderList[floorNumber].dir_down = 0;
	}
	if (stopOrder){
		stop(floorNumber);
		return;
	}

	// her be problems!
	if(!checkIsOrderedInCurrentDir(floorNumber)){
		stop(floorNumber);
	}
	//temp solution:
	//if((floorNumber == 0 && elevStateMachine.direction == DIRN_DOWN)|| (floorNumber == N_FLOORS-1 && elevStateMachine.direction == DIRN_UP)){
	//	stop(floorNumber);
	//}
}


void atFloorActions(){

	 printf("entering atFloorActions\n");
	int floorNumber = elev_get_floor_sensor_signal();

	
	if(floorNumber != -1){
		elev_set_floor_indicator(floorNumber);
		
		checkForStop(floorNumber);
	}




	//Skal erstattes med bedre kode
	/*if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
	        elev_set_motor_direction(DIRN_DOWN);
	} else if (elev_get_floor_sensor_signal() == 0) {
	        elev_set_motor_direction(DIRN_UP);
	    }*/
}