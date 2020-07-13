/// file: DigitalIO.h header for DigitalIO class  
/// author: Bennett Cook
/// date: 07-11-2020
/// description: 


// header guard
#ifndef DIGITALIO_H
#define DIGITALIO_H

#include <string>
#include <map>
#include <vector>

#include <wiringPi.h>

using namespace std;

class DigitalIO {

public: 
  DigitalIO();
  ~DigitalIO();

  int SetInputNameAndPin(string name, unsigned pin, unsigned logicLevel);
  int SetOutputNameAndPin(string name, unsigned pin);
  int ConfigureHardware();

  int ReadInputs(const vector<string> &inputNames, vector<unsigned> &values);
  int SetOutputs(const vector<string> &outputNames, const vector<unsigned> &values);

  string GetStatusStr(){ return _statusStr; }

private:

  map<string, tuple<unsigned, unsigned>> _inputs; 
  map<string, unsigned> _outputs;

  string _statusStr; 

}; // end class


#endif // end header guard
