
#include "TestSingleton.h"


TestSingleton::TestSingleton(const string &name){
   _name = name;
   _sem = nullptr;
} // ctor 

TestSingleton::~TestSingleton(){

   if(_sem != nullptr){
      sem_close(_sem);
   } // end if 

} // dtor 

bool TestSingleton::IsSingleton(){
   bool ret = true;
   int *val = nullptr;

   _sem = sem_open(_name.c_str(), O_CREAT | O_EXCL);
   sem_getvalue(_sem, val);
   if(*val == EACCES || *val == EEXIST) {
      ret = false;
      _sem = nullptr;
   } // end if 

   return ret;
} // end IsSingleton
