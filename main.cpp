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

using namespace std; 


int main(int argc, char* argv[]) {

   ReadConfigurationFile rcf;

   // check and convert command line params   
   ParseCommandLine pcl;
   int result = pcl.Parse(int argc, char *argv[]); 
   if(result != 0) {
      cout << "command line error: " << pcl.GetErrorString() <<  << endl;
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

   int done = 100;
   while(done) {

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

            result = udb.AddRow(GetSqlite3DateTime(), static_cast<int>(state));
            if(result != 0){
               cout << "database write error: " << udb.GetErrorStr() << endl; 
               return 0;
            } // end if 

            ioValues["door_cycling"] = 0;
            PrintIo(ioValues);
            cout << "open\n";

            break;
         case DoorState::Closed:     

            result = udb.AddRow(GetSqlite3DateTime(), static_cast<int>(state));
            if(result != 0){
               cout << "database write error: " << udb.GetErrorStr() << endl; 
               return 0;
            } // end if 

            ioValues["door_cycling"] = 0;
            PrintIo(ioValues);
            cout << "closed\n";

            break;
         case DoorState::Moving: 
 
            result = udb.AddRow(GetSqlite3DateTime(), static_cast<int>(state));
            if(result != 0){
               cout << "database write error: " << udb.GetErrorStr() << endl"; 
               return 0;
            } // end if 

            ioValues["door_cycling"] = 1;

            PrintIo(ioValues);
            cout << "moving\n";

            break;
         case DoorState::None: 
            cout << "*";
            cout.flush();
            break;
         case DoorState::NoChange:   
            cout << ".";
            cout.flush();
            // do nothing on these cases, it fixes complier warnings   
             break;
 
         } // end switch

         // set the outputs 
         digitalIo.SetOutputs(ioValues);
 
      }
      else {
         cout << "door sensor error: " << ds.GetErrorStr() << endl; 
         return 0;
      } // end if 

      this_thread::sleep_for(chrono::milliseconds(ac.loopTimeMS));

//       done--;
   } // end while 

   // turn off the moving led 
   ioValues["door_cycling"] = 0;
   digitalIo.SetOutputs(ioValues);

   return 0;
} // end main
