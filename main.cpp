// 7-5-2020
// desc: example mostly from the SQlite3 documentation 


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>

#include "CommonDef.h"
#include "Util.h"
#include "ReadConfigurationFile.h"
#include "UpdateDatabase.h"
#include "DigitalIO.h"
#include "DoorSensor.h"
#include "ParseCommandLine.h"
#include <boost/filesystem.hpp>
#include "ProcessCount.h"

using namespace std; 
namespace filesys = boost::filesystem;


int main(int argc, char* argv[]) {

   cout << "gdoor started with " << argc << " params" <<  endl;

   string exeName;
   filesys::path exePath(argv[0]);
   if(exePath.has_stem()) {
     exeName = exePath.stem().string();
   }
   else {
     exeName = exePath.filename().string();
   } // end if 

   ProcessCount pc(exeName);
   if(pc.GetCount() > 1){
      cout <<  "another instance of " << argv[0] << " is running, exiting" << endl;
      return 0;
   } // end if 

   ReadConfigurationFile rcf;

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

   rcf.SetConfigFilename(pcl.GetConfigFile());

   result = rcf.ReadIn();
   if(result != 0) {
      cout << "configuration file read in error: " <<  rcf.GetErrorStr() << endl;
      return 0;
   } // end if 

   AppConfig ac = rcf.GetConfiguration();
   UpdateDatabase udb;
   udb.SetDbFullPath(ac.dbPath);

   // make a digial io class and configure digial io points
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

   while(true) {
      static int readTemp = 0;

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
