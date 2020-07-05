// file: ParseCommandLine.h header for ParseCommandLine
// class: CS6030 Bioinfomatics Algorithms
// assignment: project, Hidden Markov Models
// author: Bennett Cook
// email: bennett.j.cook@wmich.edu
// date: 11-21-2019
// 

// header guard
#ifndef PARSECOMMANDLINE_H
#define PARSECOMMANDLINE_H

#include <string>

#include "CommonDef.h"
#include "Utils.h"

using namespace std;


/// \class
/// \brief class utility class to parse the command line param
class ParseCommandLine {

public:

   /// \brief constructor
   ParseCommandLine();

   /// \brief destructor
   ~ParseCommandLine();

   /// \brief parse the command line args for the input file and cost values
   /// \param argc the program's cl argument count  
   /// \param argv the program's cl argument strings 
   /// \ret 0 success
   /// \ret -1 invalid or missing cl arguments and the error string is set
   int Parse(int argc, char *argv[]);

   /// \brief getter functions
   string GetConfigFilename() { return configFile_; }
   string GetInputFilename() { return inputFile_; }
   string GetTrainingFilename() { return trainingFile_; }
   string GetErrorString() { return error_;}

private:

   string configFile_;    // the config file path
   string inputFile_;     // the input file path
   string trainingFile_;  // the training file path
   string error_;         // parsing error string

}; // end class

#endif // end header guard