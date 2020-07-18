

#include "DoorSensor.h"

DoorSensor::DoorSensor(){
   _lastState = DoorState::None;
   _currentState = DoorState::None;
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

int DoorSensor::Process(IoValues &ioValues){
   int ret = 0;

   switch(_currentState){
      case DoorState::None: 

         if(ioValues["up_position"] == 1){
            _currentState = DoorState::Open;
         }
         else if(ioValues["down_position"] == 1){
            _currentState = DoorState::Closed;
         } // end if 

         break;
      case DoorState::Open: 

         if(ioValues["up_position"] == 1){
             _currentState = DoorState::Open;
         }
         else {
            _currentState = DoorState::Moving;
         } // end

         break;
      case DoorState::Closed:     

         if(ioValues["down_position"] == 1){
             _currentState = DoorState::Closed;
         }
         else {
            _currentState = DoorState::Moving;
         } // end

         break;
      case DoorState::Moving: 

         if(ioValues["up_position"] == 1){
             _currentState = DoorState::Open;
         }
         else if(ioValues["down_position"] == 1){
            _currentState = DoorState::Closed;
         } // end if 

         break;
   } // end switch
   
   return ret;
} // end Process

DoorState DoorSensor::GetState() { 

   if(_lastState == _currentState){
      return DoorState::NoChange;
   }
   else {
      _lastState = _currentState;
   } // end if 

   return _currentState; 
} // end GetState