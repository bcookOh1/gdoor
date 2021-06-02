
// header guard
#ifndef HELPLIGHTREADER_H
#define HELPLIGHTREADER_H

#include <iostream>
#include <iomanip>
#include <string>

#include "Reader.h"

using namespace std;

class HelpLightReader : public Reader {
public:

   HelpLightReader();
   virtual ~HelpLightReader();

   int ReadAsnyc() override;

private:


}; // end class


#endif // end header guard
