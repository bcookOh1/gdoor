#include "HelpLightReader.h"


HelpLightReader::HelpLightReader() {

} // end ctor 

HelpLightReader::~HelpLightReader() {

} // end dtor 


int HelpLightReader::ReadAsnyc() {
   int ret = 0;

   // cout << "in HelpLightReader::ReadAsnyc() " << endl;

   // required call to parent 
   Reader::SetStatus(ReaderStatus::Complete, "no error");

   return ret;
} // end ReadAsnyc
