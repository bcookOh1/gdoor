// file ProcessCount.h
// author: BCook
// date: 08/20/2020 
// description: used in linux/unix only, test if an instance of this program already running.
// Because the gdoor uses gpio for input/output there can on be one instance running, the
// GetCount() member returns the number of running processes for the constructor param
// "name". The caller then decides the number of processes to allow. 
// remarks: This doesn't actually need to be a class since GetCount() does all of the work, but 
// ProcessCount evolved from an earlier version and may change again so the class was kept 

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

// class to read the number of running instances for process name
class ProcessCount {
public:

   // constructor, sets the name fo the process to check
   ProcessCount(const string &name);
   ~ProcessCount();

   // returns the number running process called _name.  
   int GetCount();

private:

   // the process name for the numbr of instances to count  
   string _name;

}; // end ProcessCount 

#endif // end header guard
