////////////////////////////////////
//TTK4235 Tilpassede datasystemer //
//Heisprosjekt                    //
//Henrik Bjering Strand           //
//Håkon Yssen Rørstad             //
////////////////////////////////////

#pragma once
#include "elev.h"
#include "elevEventHandler.h"
#include "timer.h"

//Struct containing different states for each button at a floor
struct FloorOrder {
	int dir_up;
	int dir_down;
	int inside_command;
	int floorOrdered;
};


struct StateMachine {
	struct FloorOrder orderList[N_FLOORS];
	elev_motor_direction_t direction;
	int lastFloor;
};

//Initialize global struct for the statemachine
struct StateMachine elevStateMachine;


void setFloorOrder(int floor, elev_button_type_t buttonType);
void initStateMachine();
void stop(int floorNumber);
void start(elev_motor_direction_t direction);
int  checkForOrderInCurrentDir(int floorNumber);
void stopButtonPressed();