
#include "Reader.h"

Reader::Reader() {
   _status = ReaderStatus::NotStarted;
   _stopWait = false;
} // end ctor 

Reader::~Reader() {
} // end dtor 


void Reader::SetStatus(ReaderStatus status, string errorStr) {
   _status = status;
   _errorStr = errorStr;
} // end SetStatus


void Reader::SetError(const string &errorStr) {
   _status = ReaderStatus::NotStarted; 
   _errorStr = errorStr; 
} // end SetError


// ref: https://www.cplusplus.com/reference/thread/thread/detach/
int Reader::ReadAfterSec(unsigned sec) {
   int ret = 0;

   // allow another run to start is NotStarted
   if(ReaderStatus::Waiting != _status) {
      _status = ReaderStatus::Waiting;
      thread([this](unsigned s) {this->WaitThenRun(s); }, sec).detach();
   }
   else {
      ret = 1; // in process
   } // end if 

   return ret;
} // end ReadAfterSec


void Reader::WaitThenRun(unsigned sec) {
   unsigned count_down = sec;

   // count down for 1 second then run ReadAsnyc()
   while(count_down--) {

      // _stopWait is set from StopWaiting() is true set to false and return 
      // without running ReadAsnyc()
      if(_stopWait == true) {
         _stopWait = false;

         _status = ReaderStatus::NotStarted;
         return;
      } // end if 

      this_thread::sleep_for(chrono::seconds(1));
   } // end while
   
   ReadAsnyc();
} // end WaitThenRun



