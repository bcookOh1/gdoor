
// header guard
#ifndef PITEMPREADER_H
#define PITEMPREADER_H

#include <iostream>
#include <iomanip>
#include <string>

#include "Reader.h"

using namespace std;

class PiTempReader : public Reader {
public:

   PiTempReader();
   virtual ~PiTempReader();

   int ReadAsnyc() override;
   string GetData();


private:


}; // end class


#endif // end header guard
