/// file: CommonDef.h
/// author: Bennett Cook
/// date: 07-05-2020
/// description: a place to put common program definitions. 
/// update: 05-27-2021: add help light on time constants


// header guard
#ifndef COMMONDEF_H
#define COMMONDEF_H

#include <string>
#include <vector>
#include <map>
#include <tuple>

#include <wiringPi.h>

using namespace std;


// define an enum for the gpio type
// values match macros in wiringPi.h
enum class PinType {
   DInput = 0,
   DOutput
}; 

// define an enum for the digital input resistor
// values match macros in wiringPi.h
enum class InputResistorMode {
   None = 0,
   PullDown,
   PullUp
}; 

enum class DoorState : int {
   Startup = 0,
   Open,
   MovingToClose,
   Closed,
   MovingToOpen,
   NoChange
}; // end enum 


// alias for setting or reading io values 
//                  name,    value  
using IoValues = map<string, unsigned>;

// configuration file data names
const string CONFIG_APP_NAME = "GarageDoorPi.name";
const string CONFIG_DB_PATH = "GarageDoorPi.database_path";
const string CONFIG_DB_DOOR_STATE_TABLE = "GarageDoorPi.door_state_table";
const string CONFIG_DB_SENSOR_TABLE = "GarageDoorPi.sensor_table";
const string CONFIG_DIGITAL_IO = "GarageDoorPi.digital_io";
const string CONFIG_DIGITAL_IO_TYPE = "type";
const string CONFIG_DIGITAL_IO_NAME = "name";
const string CONFIG_DIGITAL_IO_PIN = "pin";
const string CONFIG_DIGITAL_INPUT_RESISTOR_MODE = "resistor_mode";
const string CONFIG_LOOP_TIME_MS = "GarageDoorPi.loop_time_ms";
const string CONFIG_PI_TEMP_READ_INTERVAL_SEC = "GarageDoorPi.pi_temp_read_interval_sec";
const string CONFIG_SENSOR_READ_INTERVAL_SEC = "GarageDoorPi.sensor_read_interval_sec";
const string CONFIG_HELP_LIGHT_ON_TIME_SEC = "GarageDoorPi.help_light_on_time_sec";

// string values for digital io type 
const string DIGITAL_INPUT_STR = "input";
const string DIGITAL_OUTPUT_STR = "output";

// string values for digital input resistor mode 
const string INPUT_RESISTOR_PULLDOWN_STR = "pulldown";
const string INPUT_RESISTOR_PULLUP_STR = "pullup";


// define a copyable struct for gpio configurations 
struct IoConfig {

   IoConfig() {
      type = PinType::DInput,
      pin = 0;
      resistor_mode = InputResistorMode::None;
   } // end ctor

   // copy constructor
   IoConfig(const IoConfig &rhs) {
      type = rhs.type;
      name = rhs.name;
      pin = rhs.pin;
      resistor_mode = rhs.resistor_mode; 
   } // end ctor

   // assignment operator
   IoConfig &operator=(const IoConfig &rhs) {
      type = rhs.type;
      name = rhs.name;
      pin = rhs.pin;
      resistor_mode = rhs.resistor_mode;
      return *this;
   } // end assignment operator

   // set type from string 
   void SetTypeFromString(const string &str) {
      if(str == DIGITAL_OUTPUT_STR){
         type = PinType::DOutput;
      }
      else {
         type = PinType::DInput;
      } // end if 
   } // end SetTypeFromString 

   // set mode from string 
   void SetInputResistorModeFromString(const string &str) {
      if(str == INPUT_RESISTOR_PULLDOWN_STR){
         resistor_mode = InputResistorMode::PullDown;
      }
      else if(str == INPUT_RESISTOR_PULLUP_STR){
         resistor_mode = InputResistorMode::PullUp;
      }
      else {
         resistor_mode = InputResistorMode::None;
      } // end if 
   } // end SetTypeFromString 

   PinType type;
   string name; 
   unsigned pin;
   InputResistorMode resistor_mode;
}; // end struct

// simple struct with application configuration.
// the struct is copyable with copy constructor
// and assignment operator 
struct AppConfig  {

   // default constructor
   AppConfig() { loopTimeMS = 0; 
                 sensorReadIntervalSec = 0; 
                 helpLightOnTimeSec = 0;
                 piTempReadIntervalSec = 0;
   } // end ctor 

   // copy constructor
   AppConfig(const AppConfig &rhs) {
      appName = rhs.appName;
      dbPath = rhs.dbPath;
      dbDoorStateTable = rhs.dbDoorStateTable;
      dbSensorTable = rhs.dbSensorTable;
      dIos = rhs.dIos;
      loopTimeMS = rhs.loopTimeMS;
      piTempReadIntervalSec = rhs.piTempReadIntervalSec;
      sensorReadIntervalSec = rhs.sensorReadIntervalSec;
      helpLightOnTimeSec = rhs.helpLightOnTimeSec;
   } // end ctor

   // assignment operator 
   AppConfig &operator=(const AppConfig &rhs) {
      appName = rhs.appName;
      dbPath = rhs.dbPath;
      dbDoorStateTable = rhs.dbDoorStateTable;
      dbSensorTable = rhs.dbSensorTable;
      dIos = rhs.dIos;
      loopTimeMS = rhs.loopTimeMS;
      piTempReadIntervalSec = rhs.piTempReadIntervalSec;
      sensorReadIntervalSec = rhs.sensorReadIntervalSec;
      helpLightOnTimeSec = rhs.helpLightOnTimeSec;
      return *this;
   } // assignment operator

   /// \brief function to initial struct members 
   void Initialize() {
      appName = "";
      dbPath = "";
      dbDoorStateTable = "";
      dbSensorTable = "";
      dIos.clear();
      loopTimeMS = 0;
      piTempReadIntervalSec = 0;
      sensorReadIntervalSec = 0;
      helpLightOnTimeSec = 0;
   } // end Initialize

   string appName;               /// application name 
   string dbPath;                /// the full path to the shared databased 
   string dbDoorStateTable;      /// name of the door state table 
   string dbSensorTable;         /// name of the sensor reading table 
   vector<IoConfig> dIos;        /// a list of the digital io points 
   int loopTimeMS;               /// the program's read input loop time in ms
   int piTempReadIntervalSec;    /// the PI temperature read period in seconds 
   int sensorReadIntervalSec;    /// the sensor read interval in seconds 
   int helpLightOnTimeSec;       /// help light on time in seconds  

}; // end struct 


#endif // end header guard