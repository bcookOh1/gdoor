
#include "DigitalIO.h"

DigitalIO::DigitalIO(){
  wiringPiSetup();	// Initialize wiringPi
} // end ctor 
  
DigitalIO::~DigitalIO(){
} // end dtor 


int DigitalIO::SetupIoPoints(const vector<IoConfig> &dioVect){
  int ret = 0;

  for(auto iter = dioVect.begin(); iter != dioVect.end(); ++iter){

     

// map<string, IoConfig> _dios; 
// set<unsigned> _pinList;

  } // end for 


  return ret;
} // end SetupIoPoints

int DigitalIO::ConfigureHardware(){
  int ret = 0;

  // setup inputs 
  for(auto iter = _inputs.begin(); iter != _inputs.end(); ++iter){
    pinMode((*iter)->first.c_str(), get<0>((*iter)->second), get<1>((*iter)->second));
    pullUpDnControl((*iter)->first.c_str(), get<2>((*iter)->second));  
  } // end for

  return ret;
} // end ConfigureHardware


int DigitalIO::ReadAll(IoValues &inputs){
  int ret = 0;
  
  // read the requested inputs and return in value vect
  for(auto iter = inputs.begin();  iter != inputs.end(); ++iter){


     inputs.push_back(digitalRead(*iter));
  
  } // end for 

  return ret;
} // end ReadInputs



int DigitalIO::SetOutputs(const IoValues &values){
    int ret = 0;

    // digitalWrite(ledPin, LOW);

    return ret;
} // end ReadInputs

// use the name to get the pin number from the defined inputs or outputs
int DigitalIO::GetPinForName(const string &name, unsigned &pin){
    int ret = 0;

    auto inputsIter = _dios.find(name);
    if(inputsIter != _inputs.end()){
       pin = get<0>(inputsIter->second);
    }
    else {
      auto outputsIter = _outputs.find(name);
      if(outputsIter != _outputs.end()){
         pin = outputIter->second;
      }
      else {
         BOOST_ASSERT_MSG(false, "pin number from name not found");
         ret = -1;
      } // end if 
    } // end if 

    return ret;
} // end GetPinFromName
