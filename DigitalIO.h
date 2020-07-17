/// file: DigitalIO.h header for DigitalIO class  
/// author: Bennett Cook
/// date: 07-11-2020
/// description: 


// header guard
#ifndef DIGITALIO_H
#define DIGITALIO_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include <boost/assert.hpp>

#include "CommonDef.h"
#include "Util.h"

#include <wiringPi.h>

using namespace std;
using namespace boost;

class DigitalIO {

public: 
   DigitalIO();
   ~DigitalIO();

   int SetIoPoints(const vector<IoConfig> &dioVect);
   int ConfigureHardware();

   int ReadAll(IoValues &values);
   int SetOutputs(const IoValues &values);

   string GetErrorStr(){ return _errorStr; }

private:

   map<string, IoConfig> _dios; 
   string _errorStr; 

   int GetPinForName(const string &name, unsigned &pin);

}; // end class


#endif // end header guard
