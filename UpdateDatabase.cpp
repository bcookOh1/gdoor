
#include "UpdateDatabase.h"


UpdateDatabase::UpdateDatabase(){
   _errorStr = "success";
} // end ctor 


UpdateDatabase::~UpdateDatabase() {
} // end dtor 


int UpdateDatabase::SetDbFullPath(const string &fullPath) {
    int ret = 0;
    _dbFullPath = fullPath;
    return ret;
  } // end SetDbFilename


int UpdateDatabase::SetDbTableName(const string &dbTable) {
   int ret = 0;
   _dbTable = dbTable;
   return ret;
} // end SetDbTableName


int UpdateDatabase::AddRow(string start, int start_state, string end, int end_state)  {
   int ret = 0;
   
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   std::string sql;


   // open db use full path 
   rc = sqlite3_open(_dbFullPath.c_str(), &db);
   if(rc) {
      _errorStr = "can't open database: ";
      _errorStr += sqlite3_errmsg(db);
      sqlite3_free(zErrMsg);
      return -1;
   } // end if 
 

   // execute sql statement 
   rc = sqlite3_exec(db, "begin", callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      _errorStr = "begin command error: ";
      _errorStr += sqlite3_errmsg(db);
      sqlite3_free(zErrMsg);
      sqlite3_close(db);
      return -1;
   } // end if 


   // build a insert sql command 
   sql = "insert into door_state values ";
   sql += "('" + start + "'," + lexical_cast<string>(start_state) + ",'";
   sql +=  end + "'," + lexical_cast<string>(end_state) + ")";

   // execute sql statement to insert a row
   rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      _errorStr = "insert row: ";
      _errorStr += sqlite3_errmsg(db);
      sqlite3_free(zErrMsg);
      sqlite3_close(db);
      return -1;
   } // end if 


   // execute end (commit) transition 
   rc = sqlite3_exec(db, "end", callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      _errorStr = "begin command error: ";
      _errorStr += sqlite3_errmsg(db);
      sqlite3_free(zErrMsg);
      sqlite3_close(db);
      return -1;
   } // end if 
   
   sqlite3_close(db);

   return ret;
} // end AddRow



