
#include "PiTempReader.h"


PiTempReader::PiTempReader() {

} // end ctor 

PiTempReader::~PiTempReader() {

} // end dtor 

int PiTempReader::ReadAsnyc() {
   int ret = 0;

   // cout << "in PiTempReader::ReadAsnyc() " << endl;

   // required call to parent 
   Reader::SetStatus(ReaderStatus::Complete, "no error");

   return ret;
} // end ReadAsnyc

string PiTempReader::GetData() {
   string ret;

   return ret;
} // end GetData



