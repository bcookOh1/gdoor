
#include "DigitalIO.h"

DigitalIO::DigitalIO(){
   wiringPiSetup();	// Initialize wiringPi
} // end ctor 
  
DigitalIO::~DigitalIO(){
} // end dtor 


int DigitalIO::SetIoPoints(const vector<IoConfig> &dioVect){
   int ret = 0;

   _dios.clear();
   set<unsigned> pinList;

   for(auto iter = dioVect.begin(); iter != dioVect.end(); ++iter){
      IoConfig ioconfig = (*iter);

      auto result = _dios.insert(make_pair(ioconfig.name, ioconfig));
      if(result.second == false){
         _errorStr = "duplicate IO name: " + ioconfig.name;
         return -1;
      } // end if 

      auto result = pinList.insert(ioconfig.pin);
      if(result.second == false){
         _errorStr = "duplicate pin number: " + ioconfig.pin;
         return -1;
      } // end if 
     
   } // end for 

   return ret;
} // end SetupIoPoints

int DigitalIO::ConfigureHardware(){
   int ret = 0;

   // setup digital io inputs 
   for(auto iter = _dios.begin(); iter != _dios.end(); ++iter){
      pinMode(iter->second.pin, static_cast<int>(iter->second.type));
      if(iter->second.type == PinType::DInput){
         pullUpDnControl(iter->second.pin, static_cast<int>(iter->second.resistor_mode));  
      } // end if 
   } // end for

   return ret;
} // end ConfigureHardware


int DigitalIO::ReadAll(IoValues &io){
   int ret = 0;
  
   // read the requested inputs and return in value vect
   for(auto iter = io.begin();  iter != io.end(); ++iter){

      unsigned pin = 0;
      int result = GetPinForName(iter->first, pin);
      if(result != 0){
         _errorStr = "IO name not found: " + iter->first; 
         return -1;
      } // end if 

      iter->second = static_cast<unsigned>(digitalRead(pin));
  
   } // end for 

   return ret;
} // end ReadInputs


int DigitalIO::SetOutputs(const IoValues &douts){
    int ret = 0;

   // read the requested inputs and return in value vect
   for(auto iter = douts.begin();  iter != douts.end(); ++iter){

      unsigned pin = 0;
      int result = GetPinForName(iter->first, pin);
      if(result != 0){
         _errorStr = "Output name not found: " + iter->first; 
         return -1;
      } // end if 

      digitalWrite(pin, static_cast<int>(iter->second));
  
   } // end for 

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
