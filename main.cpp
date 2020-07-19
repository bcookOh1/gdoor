// 7-5-2020
// desc: example mostly from the SQlite3 documentation 


#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <stdlib.h>

#include "CommonDef.h"
#include "Util.h"
#include "ReadConfigurationFile.h"
#include "UpdateDatabase.h"
#include "DigitalIO.h"
#include "DoorSensor.h"

using namespace std; 


int main(int argc, char* argv[]) {

   ReadConfigurationFile rcf;

   // check and convert command line params    
   if(argc == 2) {
      rcf.SetConfigFilename(argv[1]);
   }
   else {
      cout << "expected a configuration file as a parameter\n";
      return 0;
   } // end if 

   int result = rcf.ReadIn();
   if(result != 0) {
      cout << "configuration file read in error: " <<  rcf.GetErrorStr() << "\n";
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

   int done = 10;
   while(done) {

      // read the all io points 
      result = digitalIo.ReadAll(ioValues);
      if(result != 0){
        cout << "read gpio error: " << digitalIo.GetErrorStr() << "\n"; 
        return 0;
      } // end if 

      // solve the logic 
      result = ds.Process(ioValues);
      if(result == 0) {

         // set the result 
         DoorState state = ds.GetState();
         switch(state){
         case DoorState::Open: 
         case DoorState::Closed:     
         case DoorState::Moving: 
         {

            string rec_time = GetSqlite3DateTime();
            result = udb.AddRow(rec_time, static_cast<int>(state));
            if(result != 0){
            cout << "database write error: " << udb.GetErrorStr() << "\n"; 
            return 0;
            } // end if 

         }
             break;
         } // end switch
      }
      else {
         cout << "door sensor error: " << ds.GetErrorStr() << "\n"; 
         return 0;
      } // end if 

      // set the outouts 
      if(state == DoorState::Moving){
         ioValues["door_cycling"] = 1;
      }
      else 
         ioValues["door_cycling"] = 0;
      } // end if 
      digitalIo.SetOutputs(ioValues);

      done--;
   } // end while 

   // turn off the moving led 
   ioValues["door_cycling"] = 0;
   digitalIo.SetOutputs(ioValues);

   return 0;
} // end main
