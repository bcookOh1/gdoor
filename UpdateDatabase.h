

// file: UpdateDatabase.h header for UpdateDatabase
// author: Bennett Cook
// date: 07-05-2020
// 

// header guard
#ifndef UPDATEDATABASE_H
#define UPDATEDATABASE_H

#include "sqlite3.h"
#include "CommonDef.h"

#include <string>
#include <tuple>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;


class UpdateDatabase {

public:

  UpdateDatabase();
  ~UpdateDatabase();

  int SetDbFullPath(const string &fullPath);
  int SetDbTableName(const string &dbTable);
  int AddRow(string rec_time, int state);
  string GetErrorStr() { return _errorStr; }

private: 

  string _dbFullPath;
  string _dbTable;
  string _errorStr;


   // example from documentation
   static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
      int i;
      for(i = 0; i<argc; i++) {
         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      }
      printf("\n");
      return 0;
   } // end error callback
 
}; // end class

#endif // end header guard