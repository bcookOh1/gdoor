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
      cout << "configuration file read in error: " <<  rcf.GetStatusStr() << "\n";
      return 0;
   } // end if 

   AppConfig ac = rcf.GetConfiguration();
   UpdateDatabase udb;
   udb.SetDbFullPath(ac.dbPath);

   IoValues dInputValues = MakeIoValuesMap(ac.dInputs);
   IoValues dOutputValues = MakeIoValuesMap(ac.dOutputs);


   int done = 10;
   while(done) {

      string start = GetSqlite3DateTime();
      this_thread::sleep_for(chrono::milliseconds(ac.loopTimeMS));
      string end = GetSqlite3DateTime();
      result = udb.AddRow(start, 1, end, 2);
      if(result != 0){
        cout << "database write error: " << udb.GetErrorStr() << "\n"; 
        return 0;
      } // end if 

      done--;
   } // end while 

   return 0;
} // end main
