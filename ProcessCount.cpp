
#include "ProcessCount.h"


ProcessCount::ProcessCount(const string &name){
   _name = name;
} // ctor 

ProcessCount::~ProcessCount(){
} // dtor 

// ref: https://www.boost.org/doc/libs/1_74_0/doc/html/process.html#boost_process.introduction
int ProcessCount::GetCount(){

   ostringstream oss;
   ipstream pipe_stream;
   string cmd = "pidof ";
   cmd += _name;

   child c(cmd.c_str(), std_out > pipe_stream);
   this_thread::sleep_for(chrono::milliseconds(1000));

   string line;

   while (pipe_stream && getline(pipe_stream, line) && !line.empty()) {
      oss << line << " ";
   } // end while 

   c.wait();

   string output = oss.str();
   trim(output);

   vector<string> splitVec;
   split(splitVec, output, is_any_of(" ")); 

   return static_cast<int>(splitVec.size());
} // end GetCount

