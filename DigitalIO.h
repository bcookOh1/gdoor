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

using namespace std;

class DigitalIO {

public: 
  DigitalIO();
  ~DigitalIO();


private:

  map<string,unsigned> _inputs; 
  map<string,unsigned> _outputs;


 

}; // end class


#endif // end header guard
