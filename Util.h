
// file utils.h
// author: BCook
// date: 07/06/2020 
// description: header file for the sql utilites 


// header guard
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cassert>
#include <type_traits>

#include "CommonDef.h"


using namespace std;

// return a date string in standard format "YYYY-MM-DD HH:MM:SS" with
// the current date and time  
string GetSqlite3DateTime();

// 
IoValues MakeIoValuesMap(const vector<IoConfig> &io);
void PrintIo(const IoValues &ioValues);
int ReadBoardTemperature(string &temperature);


// for use in a loop to detect when a var makes a 
// transition (edge detect)
// for now it only compiles with int or float types
template<typename T>
class OneShot {
public:
   OneShot(T value) : _value{value} {
      
      // check that only ints or floats are used
      // so the compare and assignment in Changed() works
      if(!is_integral_v<T> && !is_floating_point_v<T> ) {
         assert(false);
      } // end if 

   } // end ctor

   bool Changed(T value) {

      // look for changed value
      if(value != _value) {
         _value = value;
         return true;
      } // end if 

      return false;
   } // end Changed

private:
   T _value;

}; // end class

#endif // end header guard
