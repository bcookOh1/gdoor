

#include "DoorSensor.h"

DoorSensor::DoorSensor(){
   _lastState = DoorState::None;
   _currentState = DoorState::None;
} // end ctor 

DoorSensor::~DoorSensor(){
} // end dtor 

int DoorSensor::Process(IoValues &ioValues){
   int ret = 0;

   switch(_currentState){
      case DoorState::None: 

         if(ioValues["up_position"] == 1 && 
            ioValues["down_position"] == 0){
            _currentState = DoorState::Open;
         }
         else if(ioValues["down_position"] == 1 && 
                 ioValues["up_position"] == 0){
            _currentState = DoorState::Closed;
         } // end if 

         break;
      case DoorState::Open: 

         if(ioValues["up_position"] == 0){
             _currentState = DoorState::MovingToClose;
         } // end

         break;
      case DoorState::MovingToClose: 

         if(ioValues["up_position"] == 1){
             _currentState = DoorState::Open;
         }
         else if(ioValues["down_position"] == 1){
            _currentState = DoorState::Closed;
         } // end if

         break;
      case DoorState::Closed:     

         if(ioValues["down_position"] == 0){
            _currentState = DoorState::MovingToOpen;
         } // end

         break;
      case DoorState::MovingToOpen: 

         if(ioValues["up_position"] == 1){
             _currentState = DoorState::Open;
         }
         else if(ioValues["down_position"] == 1){
            _currentState = DoorState::Closed;
         } // end if 

         break;
      
      default:
         // do nothing, 
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