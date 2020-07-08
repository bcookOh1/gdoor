/// file: CommonDef.h
/// author: Bennett Cook
/// date: 07-05-2020
/// description: a place to put common program definitions. 
/// 

// header guard
#ifndef COMMONDEF_H
#define COMMONDEF_H


#include <string>
#include <vector>
#include <tuple>

using namespace std;

// alias for error return values 
using ErrorRet = std::tuple<int, string>;

// configuration file data names
const string CONFIG_APP_NAME = "GarageDoorProject.name";
const string CONFIG_DB_PATH = "GarageDoorProject.database_path";
const string CONFIG_DB_TABLE = "GarageDoorProject.database_table";
const string CONFIG_DIGITAL_INPUTS = "GarageDoorProject.digital_inputs";
const string CONFIG_DIGITAL_INPUT_LABELS = "GarageDoorProject.digital_input_labels";
const string CONFIG_LOOP_TIME_MS = "GarageDoorProject.loop_time_ms";


// simple struct with application configuration.
// the struct is copyable with copy constructor
// and assignment operator 
struct AppConfig  {

   // default constructor
   AppConfig() { loopTimeMS = 0;}

   // copy constructor
   AppConfig(const AppConfig &rhs) {
      appName = rhs.appName;
      dbPath = rhs.dbPath;
      dbTable = rhs.dbTable;
      dInputs = rhs.dInputs;
      dInputLabels = rhs.dInputLabels;
      loopTimeMS = rhs.loopTimeMS;
   } // end ctor

   // assignment operator 
   AppConfig &operator=(const AppConfig &rhs) {
      appName = rhs.appName;
      dbPath = rhs.dbPath;
      dbTable = rhs.dbTable;
      dInputLabels = rhs.dInputLabels;
      loopTimeMS = rhs.loopTimeMS;
      return *this;
   } // assignment operator

   /// \brief function to initial struct members 
   void Initialize() {
      appName = "";
      dbPath = "";
      dbTable = "";
      dInputs.clear();
      dInputLabels.clear();
      loopTimeMS = 0;
   } // end Initialize

   string appName;               /// application name 
   string dbPath;                /// the full path to the shared databased 
   string dbTable;               /// name of the status table 
   vector<unsigned> dInputs;     /// a list of the digital inputs 
   vector<string> dInputLabels;  /// a list of the digital input labels 
   int loopTimeMS;               /// the programs read input loop time

}; // end struct 


#endif // end header guard