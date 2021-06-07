// file: main.cpp
// date: 08/20/2020 
// description: entry point for the gdoor raspberry pi zero application. 
// update: 5/30/2021, add Help Light
// update: 5/31/2021, add PiTempReader


#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <chrono>
#include <boost/filesystem.hpp>
 
#include "CommonDef.h"
#include "Util.h"
#include "ProcessCount.h"
#include "ParseCommandLine.h"
#include "ReadConfigurationFile.h"
#include "UpdateDatabase.h"
#include "DigitalIO.h"
#include "StateMachine.hpp"
#include "HelpLightReader.h"
#include "PiTempReader.h"
#include "Si7021Reader.h"


using namespace std; 
namespace filesys = boost::filesystem;
namespace sml = boost::sml;
using Gdsm = sm_garage_door;


// entry point for the program
// usage: ./gdoor [-h] -c <config_file> [-s] 
// -h, optional, shows this help text, if included other arguments are ignored
// -c <config_file>, a json file with the configuration 
// -s, optional, the io and state information is not printed but error messages are 
// Note: a space is required between -c and the config file 
// example: ./gdoor -c config_1.json -s 
int main(int argc, char* argv[]) {

   cout << "gdoor started with " << argc << " params" <<  endl;

   // get and clean the exe name, used for process count
   string exeName;
   filesys::path exePath(argv[0]);
   if(exePath.has_stem()) {
     exeName = exePath.stem().string();
   }
   else {
     exeName = exePath.filename().string();
   } // end if 

   // get the process count for this program, exit if another instantance is running
   ProcessCount pc(exeName);
   if(pc.GetCount() > 1){
      cout <<  "another instance of " << exeName << " is running, exiting" << endl;
      return 0;
   } // end if 
 
   // check and convert command line params   
   ParseCommandLine pcl;
   int result = pcl.Parse(argc, argv); 
   if(result != 0) {
      cout << "command line error: " << pcl.GetErrorString() <<  endl;
      return 0;
   } // end if 

   if(pcl.GetHelpFlag() == true){
      cout << pcl.GetHelpString() << endl;
      return 0;
   } // end if 

   // class to read the json config file, from command line filename 
   ReadConfigurationFile rcf;
   rcf.SetConfigFilename(pcl.GetConfigFile());

   result = rcf.ReadIn();
   if(result != 0) {
      cout << "configuration file read-in error: " <<  rcf.GetErrorStr() << endl;
      return 0;
   } // end if 

   // set the config in the AppConfig class, this is a a simple container class  
   // passed to to other classes in the app
   AppConfig ac = rcf.GetConfiguration();

   // set the sqlite3 file path in the database class
   UpdateDatabase udb;
   udb.SetDbFullPath(ac.dbPath);
   udb.SetDoorStateTableName(ac.dbDoorStateTable);
   udb.SetSensorDataTableName(ac.dbSensorTable);

   // make a digial io class and configure digital io points
   DigitalIO digitalIo;
   digitalIo.SetIoPoints(ac.dIos);
   digitalIo.ConfigureHardware();

   // setup empty IoValue map used for algo data 
   IoValues ioValues = MakeIoValuesMap(ac.dIos);

   // make sure outputs are off at start up
   ioValues["door_cycling"] = 0;
   ioValues["help_light"] = 0;
   digitalIo.SetOutputs(ioValues);

   // used for a 120vac light controlled from a relay 
   HelpLightReader hlr;

   // door_cycling oneshot, used to set help light,
   // used in the while loop, true on-change for one loop
   OneShot<unsigned> dcOs{0}; 

   PiTempReader pitr;
   Si7021Reader si7021r;

   // read the temp once at the start so the temperature var is valid
   string temperature;
   result = pitr.RunTask();
   if(result != 0){
      cout << pitr.GetError() << endl;
      return 0;
   }
   else {
      temperature = pitr.GetData();
   } // end if 
   
   // make the state machine passing in dio, update database and pi temp
   Gdsm gdsm(ioValues, udb, temperature);
   sml::sm<Gdsm> sm(gdsm);

   // move off Idle states
   sm.process_event(eGdInit{});

   // main control loop, it follows this structure:
   //  - read inputs
   //  - solve sequencing logic
   //  - set outputs/write database record
   while(true) {
      
      // read the all io points 
      result = digitalIo.ReadAll(ioValues);
      if(result != 0){
        cout << "read gpio error: " << digitalIo.GetErrorStr() << endl; 
        return 0;
      } // end if 

      // send the only event eOnTime
      // same event each loop, the state and guards make up the transitions 
      sm.process_event(eOnTime{});


      // read PI temp every n seconds
      if(pitr.GetStatus() == ReaderStatus::NotStarted){
         pitr.ReadAfterSec(ac.piTempReadIntervalSec);
      }
      else if(pitr.GetStatus() == ReaderStatus::Complete){
         temperature = pitr.GetData();
         pitr.ResetStatus();
      }
      else if(pitr.GetStatus() == ReaderStatus::Error) {
         cout << pitr.GetError() << endl;
         pitr.ResetStatus();
      } // end if 


      // read Si7021 temp and humidity every n seconds
      if(si7021r.GetStatus() == ReaderStatus::NotStarted){
         si7021r.ReadAfterSec(ac.sensorReadIntervalSec);
      }
      else if(si7021r.GetStatus() == ReaderStatus::Complete){
         Si7021Data data = si7021r.GetData();
         si7021r.ResetStatus();

         // write sensor data to db 
         int sensorReadResult = udb.AddOneSensorDataRow(GetSqlite3DateTime(),  
                                                        data.temperature,
                                                        data.TemperatureUnits,
                                                        data.humidity,
                                                        data.humidityUnits); 
         if(sensorReadResult == -1) {
            cout << udb.GetErrorStr() << endl;
         } // end if 

      }
      else if(si7021r.GetStatus() == ReaderStatus::Error) {
         cout << si7021r.GetError() << endl;
         si7021r.ResetStatus();
      } // end if 


      // set help light on/off 
      bool doorCyclingChanged = dcOs.Changed(ioValues["door_cycling"]);
      if(hlr.GetStatus() == ReaderStatus::NotStarted && doorCyclingChanged){
         hlr.ReadAfterSec(ac.helpLightOnTimeSec);
         ioValues["help_light"] = 1;
      }
      else if(hlr.GetStatus() == ReaderStatus::Waiting && doorCyclingChanged){
         hlr.RestartWait();
      }
      else if(hlr.GetStatus() == ReaderStatus::Complete){
         hlr.ResetStatus();
         ioValues["help_light"] = 0;
      } // end if 

      // set the outputs 
      digitalIo.SetOutputs(ioValues);

      this_thread::sleep_for(chrono::milliseconds(ac.loopTimeMS));

   } // end while 

   // turn off the moving led and help light
   ioValues["door_cycling"] = 0;
   ioValues["help_light"] = 0;
   digitalIo.SetOutputs(ioValues);

   return 0;
} // end main
