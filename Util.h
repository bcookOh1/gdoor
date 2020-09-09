
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

#include "CommonDef.h"


using namespace std;

// return a date string in standard format "YYYY-MM-DD HH:MM:SS" with
// the current date and time  
string GetSqlite3DateTime();

// 
IoValues MakeIoValuesMap(const vector<IoConfig> &io);
void PrintIo(const IoValues &ioValues);
int ReadBoardTemperature(string &temperature);


#endif // end header guard
