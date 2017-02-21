#include "testEventHandler.h"
#include "elevStateMachine.h"

void checkButtonsForOrder(){
	//floors
	for (int floorNumb = 0; floorNumb<N_FLOORS; floorNumb++){
		//buttonType
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
	elev_set_motor_direction(DIRN_DOWN);
	while(!(elev_get_floor_sensor_signal() == 0)){
	}
	elev_set_motor_direction(DIRN_STOP);
}

void stopAtFloor(int floorNumber){
	int stopOrder = 0;
	//could have used --
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
	}
}


void atFloorActions(){
	int floorNumber = elev_get_floor_sensor_signal();

	
	if(floorNumber != -1){
		elev_set_floor_indicator(floorNumber);
		
		stopAtFloor(floorNumber);

	}




	//Skal erstattes med bedre kode
	if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
	        elev_set_motor_direction(DIRN_DOWN);
	} else if (elev_get_floor_sensor_signal() == 0) {
	        elev_set_motor_direction(DIRN_UP);
	    }
}
