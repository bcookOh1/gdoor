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

// alias for gpio pin configuration
//                          name,   pin,      logicLevel                       
using IoConfig = std::tuple<string, unsigned, unsigned>;


// configuration file data names
const string CONFIG_APP_NAME = "GarageDoorPi.name";
const string CONFIG_DB_PATH = "GarageDoorPi.database_path";
const string CONFIG_DB_TABLE = "GarageDoorPi.database_table";
const string CONFIG_DIGITAL_INPUTS = "GarageDoorPi.digital_inputs";
const string CONFIG_DIGITAL_OUTPUTS = "GarageDoorPi.digital_outputs";
const string CONFIG_DIGITAL_IO_LABEL = "label";
const string CONFIG_DIGITAL_IO_PIN = "pin";
const string CONFIG_DIGITAL_IO_LOGIC_LEVEL = "logic_level";
const string CONFIG_LOOP_TIME_MS = "GarageDoorPi.loop_time_ms";


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
      dOutputs = rhs.dOutputs;
      loopTimeMS = rhs.loopTimeMS;
   } // end ctor

   // assignment operator 
   AppConfig &operator=(const AppConfig &rhs) {
      appName = rhs.appName;
      dbPath = rhs.dbPath;
      dbTable = rhs.dbTable;
      dInputs = rhs.dInputs;
      dOutputs = rhs.dOutputs;
      loopTimeMS = rhs.loopTimeMS;
      return *this;
   } // assignment operator

   /// \brief function to initial struct members 
   void Initialize() {
      appName = "";
      dbPath = "";
      dbTable = "";
      dInputs.clear();
      dOutputs.clear();
      loopTimeMS = 0;
   } // end Initialize

   string appName;               /// application name 
   string dbPath;                /// the full path to the shared databased 
   string dbTable;               /// name of the status table 
   vector<IoConfig> dInputs;     /// a list of the digital inputs 
   vector<IoConfig> dOutputs;    /// a list of the digital outputs 
   int loopTimeMS;               /// the programs read input loop time

}; // end struct 


#endif // end header guard