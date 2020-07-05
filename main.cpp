// 7-5-2020
// desc: example mostly from the SQlite3 documentation 


#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

// example from documentation
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
} // end error callback

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   std::string sql;

   // open db use full path 
   rc = sqlite3_open("/home/pi/projects/gdoor/garage.db", &db);  
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } 
   else {
      fprintf(stderr, "Opened database successfully\n");
   } // end if 
 
   // execute sql statement 
   rc = sqlite3_exec(db, "begin", callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
   else {
      fprintf(stdout, "begin transition\n");
   } // end if 

   // execute sql statement to insert a row
   rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
   else {
      fprintf(stdout, "success, row added\n");
   } // end if 

   // execute end (commit) transition 
   rc = sqlite3_exec(db, "end", callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } 
   else {
      fprintf(stdout, "commit transition \n");
   } // end if 
   
   sqlite3_close(db);
   
   return 0;
} // end main
