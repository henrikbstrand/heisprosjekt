#pragma once
#include "elevStateMachine.h"
#include <assert.h>


void checkButtonsForOrder();
void elevStartUp();
void atFloorActions();
void checkForStop(int floorNumber);
void checkForStart(int floorNumber);
