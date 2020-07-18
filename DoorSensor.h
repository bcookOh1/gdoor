/// file: DoorSensor.h header for DoorSensor class  
/// author: Bennett Cook
/// date: 07-17-2020
/// description: 


// header guard
#ifndef DOORSENSOR_H
#define DOORSENSOR_H

#include <string>
#include <map>
#include <vector>

#include "CommonDef.h"
#include "Util.h"

enum class DoorState {
   None = 0,
   Open,
   Closed,
   Moving,
   NoChange
}; // end enum 


class DoorSensor {

public:
   DoorSensor();
   ~DoorSensor();

   int Process(IoValues &ioValues);
   DoorState GetState();

   string GetErrorStr(){ return _errorStr; }

private:

   DoorState _lastState;
   DoorState _currentState;

   string _errorStr; 

}; // end class

#endif // end header guard