
#include "DigitalIO.h"

DigitalIO::DigitalIO(){
  wiringPiSetup();	// Initialize wiringPi
} // end ctor 
  
DigitalIO::~DigitalIO(){
} // end dtor 

int DigitalIO::ConfigureHardware(){
  int ret = 0;

  // setup inputs 
  for(auto iter = _inputs.begin(); iter != _inputs.end(); ++iter){
    pinMode((*iter)->first.c_str(), get<0>((*iter)->second), get<1>((*iter)->second));
    pullUpDnControl((*iter)->first.c_str(), get<2>((*iter)->second));  
  } // end for

  // setup outputs 
  for(auto iter = _outputs.begin(); iter != _outputs.end(); ++iter){
    pinMode((*iter)->first.c_str(), get<0>((*iter)->second), get<1>((*iter)->second));
    pullUpDnControl((*iter)->first.c_str(), get<2>((*iter)->second));  
  } // end for

  return ret;
} // end ConfigureHardware


int DigitalIO::ReadInput(IoValues &values){
  int ret = 0;
  
  // read the requested inputs and return in value vect
  for(auto iter = values.begin();  iter != _inputNames.end(); ++iter){
    values.push_back(digitalRead(*iter));
  } // end for 

  return ret;
} // end ReadInputs



int DigitalIO::SetOutputs(const vector<string> &outputNames, 
                              const vector<unsigned> &values){
    int ret = 0;

    // digitalWrite(ledPin, LOW);

    return ret;
  } // end ReadInputs

