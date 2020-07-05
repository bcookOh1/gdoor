// file: ParseCommandLine.cpp implimentation for the ParseCommandLine class
// class: CS6030 Bioinfomatics Algorithms
// assignment: project, Hidden Markov Models
// email: bennett.j.cook@wmich.edu
// date: 11-21-2019
// 

#include "ParseCommandLine.h"

/// \brief constructor
ParseCommandLine::ParseCommandLine() {
   configFile_ = "";
   inputFile_ = "";
   trainingFile_ = "";
   error_ = "";
} // end constructor 

/// \brief do nothing destructor
ParseCommandLine::~ParseCommandLine() {
} // end dtor 

/// \brief parse the command line args for the input file 
/// \ret 0 success
/// \ret -1 invalid or missing input file command line argument 
int ParseCommandLine::Parse(int argc, char *argv[]) {
   int ret = 0;

   // check minimum args 
   if(argc < kMINIMUM_ARGUMENTS)
      return -1;

   bool hasConfigFile = false;

   // set default since this is only used in the optimization hmm
   trainingFile_ = HMM_DEFAULT_TRAINING_FILENAME;

   // parse the command line arg, start with argv[1]  
   for(auto i = 1; i < argc; i++) {
      string arg(argv[i]);

      if(arg.find(kCOMMAND_LINE_CONFIG_FILE_FLAG) == 0) {

         configFile_ = arg.erase(0, kCOMMAND_LINE_CONFIG_FILE_FLAG.length());
         hasConfigFile = true;

      } // end if 
   } // end for

   // must have at least a config and input file to continue
   if(hasConfigFile == false) {
      error_ += "\nconfig filename argument missing";
      ret = -1;
   } // end if 

   return ret;
} // end Parse

