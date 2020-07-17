
#include "Util.h"


string GetSqlite3DateTime() {

   string ret;
   ostringstream oss;

   // get now time and convert to tm struct 
   time_t nowTime;
   time(&nowTime);
   struct tm *timeinfo;
   timeinfo = localtime(&nowTime);

   oss << setw(4) << (timeinfo->tm_year + 1900) << "-";
   oss << setw(2) << setfill('0') << (timeinfo->tm_mon + 1) << "-";
   oss << setw(2) << setfill('0') << timeinfo->tm_mday << " ";
   oss << setw(2) << setfill('0') << timeinfo->tm_hour << ":";
   oss << setw(2) << setfill('0') << timeinfo->tm_min << ":";
   oss << setw(2) << setfill('0') << timeinfo->tm_sec;
   ret = oss.str();

    return ret;
} // end GetSqlite3DateTime


IoValues MakeIoValuesMap(const vector<IoConfig> &io) {
   IoValues ret;

   for(auto iter = io.begin(); iter != io.end(); ++iter) {
      ret.insert(make_pair(iter->name, 0));
   } // end for 

   return ret;  
} // end MakeIoValuesMap

 