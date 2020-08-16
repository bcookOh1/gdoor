

// file TestSingleton.h
// author: BCook
// date: 08/12/2020 
// description: used in linux, test if an instance of this program already running

// header guard
#ifndef TESTSINGLETON_H
#define TESTSINGLETON_H

#include <boost/process.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>


using namespace std;
using namespace boost;
using namespace boost::process;

class ProcessCount {
public:

   ProcessCount(const string &name);
   ~ProcessCount();

   int GetCount();

private:

   string _name;

}; // end ProcessCount 

#endif // end header guard
