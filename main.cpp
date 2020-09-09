// file: main.cpp
// date: 08/20/2020 
// description: entry point for the gdoor raspberry pi zero application. 
//  


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
#include "DoorSensor.h"

using namespace std; 
namespace filesys = boost::filesystem;


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
      cout << "configuration file read in error: " <<  rcf.GetErrorStr() << endl;
      return 0;
   } // end if 

   // set the config in the AppConfig class, this is a a simple container class  
   // passed to to other classes in the app
   AppConfig ac = rcf.GetConfiguration();

   // set the sqlite3 file path in the database class
   UpdateDatabase udb;
   udb.SetDbFullPath(ac.dbPath);

   // make a digial io class and configure digital io points
   DigitalIO digitalIo;
   digitalIo.SetIoPoints(ac.dIos);
   digitalIo.ConfigureHardware();

   // setup empty IoValue map used for algo data 
   IoValues ioValues = MakeIoValuesMap(ac.dIos);
   DoorSensor ds; 

   // read the temp once at the start so the temperature var is valid
   string temperature;
   result = ReadBoardTemperature(temperature);
   if(result != 0){
      cout << "board temperature read failed " << endl;
      return 0;
   } // end if 

   // used to limit the number of times the pi temp is read
   int readTemp = 0; 

   // main control loop, it follows this structure:
   //  - read inputs
   //  - solve squencing logic
   //  - set outputs/write database record
   while(true) {
      
      // read the all io points 
      result = digitalIo.ReadAll(ioValues);
      if(result != 0){
        cout << "read gpio error: " << digitalIo.GetErrorStr() << endl; 
        return 0;
      } // end if 

      // solve the logic 
      result = ds.Process(ioValues);
      if(result == 0) {

         // set the result 
         DoorState state = ds.GetState();
         switch(state) {
         case DoorState::Open:

            result = udb.AddRow(GetSqlite3DateTime(), static_cast<int>(state), temperature);
            if(result != 0){
               cout << "database write error: " << udb.GetErrorStr() << endl; 
               return 0;
            } // end if 

            ioValues["door_cycling"] = 0;

            if(!pcl.GetSilentFlag()){
               PrintIo(ioValues);
               cout << "open" << endl;
            } // end if 

            break;
         case DoorState::Closed:     

            result = udb.AddRow(GetSqlite3DateTime(), static_cast<int>(state), temperature);
            if(result != 0){
               cout << "database write error: " << udb.GetErrorStr() << endl; 
               return 0;
            } // end if 

            ioValues["door_cycling"] = 0;

            if(!pcl.GetSilentFlag()){
               PrintIo(ioValues);
               cout << "closed" << endl;
            } // end if 

            break;
         case DoorState::MovingToOpen: 
         case DoorState::MovingToClose: 
 
            result = udb.AddRow(GetSqlite3DateTime(), static_cast<int>(state), temperature);
            if(result != 0){
               cout << "database write error: " << udb.GetErrorStr() << endl; 
               return 0;
            } // end if 

            ioValues["door_cycling"] = 1;

            if(!pcl.GetSilentFlag()){
               PrintIo(ioValues);
               if(state == DoorState::MovingToOpen)
                  cout << "moving to open" << endl;
               else 
                  cout << "moving to close" << endl;
            } // end if 
         
            break;
         case DoorState::NoChange:   
         
            if(!pcl.GetSilentFlag()){
               cout << ".";
               cout.flush();
            } // endif 

             break;
         default:

            break;
         } // end switch

         // set the outputs 
         digitalIo.SetOutputs(ioValues);
 
      }
      else {
         cout << "door sensor error: " << ds.GetErrorStr() << endl; 
         return 0;
      } // end if 

      // read temp every 10 seconds
      if(readTemp >= (ac.loopTimeMS * 1000 * 10)){
         
         // ignore the error here since is worked prior to the while 
         // and no error show occure 
         ReadBoardTemperature(temperature);
         readTemp = 0;
      }
      else {
         readTemp += ac.loopTimeMS;
      } // end if  

      this_thread::sleep_for(chrono::milliseconds(ac.loopTimeMS));

   } // end while 

   // turn off the moving led 
   ioValues["door_cycling"] = 0;
   digitalIo.SetOutputs(ioValues);

   return 0;
} // end main
