
// file: ProcessCount.h header for ProcessCount
// author: Bennett Cook
// date: 08-20-2020
// 

#include "ProcessCount.h"


ProcessCount::ProcessCount(const string &name){
   _name = name;
} // ctor 

ProcessCount::~ProcessCount(){
} // dtor 

// return the number of running processes call _name
// use the linux pidof command to get the process count. pidof  prints a space delimited 
// list of process ids for a process name
// ref: https://www.boost.org/doc/libs/1_74_0/doc/html/process.html#boost_process.introduction
int ProcessCount::GetCount(){

   ostringstream oss;       // used to accumulate the stdout response
   ipstream pipe_stream;    // used by the boost process child call 
                            // to capture the stdout response 
   string cmd = "pidof ";   // the linux command to get the process ids  
   cmd += _name;            // add the process name to the command

   // start a child process and 
   child c(cmd.c_str(), std_out > pipe_stream);

   // give the child some time to run
   this_thread::sleep_for(chrono::milliseconds(1000));

   // accumulate each stdout line to oss 
   string line;
   while (pipe_stream && getline(pipe_stream, line) && !line.empty()) {
      oss << line << " ";
   } // end while 

   // wait for thr child to finish 
   c.wait();

   // copy to output string and tgrim left/right spaces, the nunber of splitVec 
   // is the process count
   string output = oss.str();
   trim(output);

   // use boost split to make vector of process ids, 
   vector<string> splitVec;
   split(splitVec, output, is_any_of(" ")); 

   // return the number of process ids
   return static_cast<int>(splitVec.size());
} // end GetCount

