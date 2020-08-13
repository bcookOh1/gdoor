

// file TestSingleton.h
// author: BCook
// date: 08/12/2020 
// description: used in linux, test if an instance of this program already running

// header guard
#ifndef TESTSINGLETON_H
#define TESTSINGLETON_H


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include <string>

using namespace std;


class TestSingleton {
public:

   TestSingleton(const string &name);
   ~TestSingleton();

   bool IsSingleton();

private:

   sem_t *_sem;
   string _name;

}; // end TestSingleton 

#endif // end header guard