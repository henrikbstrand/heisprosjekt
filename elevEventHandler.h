////////////////////////////////////
//TTK4235 Tilpassede datasystemer //
//Heisprosjekt                    //
//Henrik Bjering Strand           //
//Håkon Yssen Rørstad             //
////////////////////////////////////

#pragma once
#include "elevStateMachine.h"
#include <assert.h>


void checkButtonsForOrder();
void elevStartUp();
void atFloorActions();
void checkForStop(int floorNumber);
void checkForStart(int floorNumber);
