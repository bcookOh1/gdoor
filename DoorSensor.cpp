

#include "DoorSensor.h"

DoorSensor::DoorSensor(){
   _lastState = DoorState::None;
} // end ctor 

DoorSensor::~DoorSensor(){
} // end dtor 

/*
enum class DoorState {
   None = 0,
   Open,
   Closed,
   MovingToOpen,
   MovingToClose
}; // end enum 

*/

int DoorSensor::Process(IoValues &values){
   int ret = 0;


   
   return ret;
} // end Process