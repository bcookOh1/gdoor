/// file: ReadConfigurationFile.h header for ReadConfigurationFile class  
/// author: Bennett Cook
/// date: 11-23-2019
/// description: read in a json file and and populate an HmmConfiguration 
/// struct. This class uses a boost::property_tree as the read-in/container 
/// for the config. But the configuration data has to be converted to 
/// the HmmConfiguration members so there are 3 template functions for 
/// the more complex conversions.


// header guard
#ifndef READCONFIGURATIONFILE_H
#define READCONFIGURATIONFILE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "CommonDef.h"
#include "Utils.h"
  
using namespace std;
using namespace boost;
namespace pt = boost::property_tree;

/// \class 
/// \brief class to read the json file 
/// and parse the data string  
class ReadConfigurationFile {
public:

   /// \brief constructor
   ReadConfigurationFile();

   /// \brief destructor
   ~ReadConfigurationFile();

   /// \brief set and get for the configuration file name 
   void SetConfigFilename(string fname) { configFilename_ = fname; }
   string GetConfigFilename() { return configFilename_; }

   /// \brief perform the jsone file read-in and convert element to 
   /// HmmConfiguration members.
   /// \return 0 success
   /// \return -1 an error occured and the error string was set
   int ReadIn();

   /// \brief get fuction for the config 
   HmmConfiguration GetConfiguration(){ return hmmConfig_; }

   /// \brief get build error string  
   string GetError() { return error_; }

private:

   string configFilename_;      // a file name  
   string error_;               // error string
   HmmConfiguration hmmConfig_; // the configuration

   /// \brief template function to read a 2d array from the property tree
   template<typename T>
   vector<vector<T>> Get2dData(pt::ptree tree, const string &child_label) {
      vector<vector<T>> ret;
      string error;

      try {
         for(pt::ptree::value_type &v : tree.get_child(child_label.c_str())) {
            vector<T> temp;
            for(pt::ptree::value_type &v2 : v.second) {
               try {
                  temp.push_back(lexical_cast<T>(v2.second.data()));
               } catch(bad_lexical_cast &e) {
                  error_ = "error on lexical_cast ";
                  error_ += e.what();
               } // end try catch

               // if exception occured throw  
               if(error.length() > 0) throw error.c_str();

            } // end for 
            ret.push_back(temp);
         } // end for 
      } 
      catch(std::exception &e) {
         error_ = "error on child read ";
         error_ += e.what();
      } // end try/catch

      // if exception occured throw  
      if(error.length() > 0) throw error_.c_str();

      return ret;
   } // end Get2dData

   /// \brief template function to read a 1d array from the property tree
   template<typename T>
   vector<T> Get1dData(pt::ptree tree, const string &child_label) {
      vector<T> ret;
      string error;

      try {
         for(pt::ptree::value_type &v : tree.get_child(child_label.c_str())) {
            try {
               ret.push_back(lexical_cast<T>(v.second.data()));
            } catch(bad_lexical_cast &e) {
               error_ = "error on lexical_cast ";
               error_ += e.what();
            } // end try catch

            // if exception occured throw  
            if(error_.length() > 0) throw error_.c_str();
         } // end for 

      } 
      catch(std::exception &e) {
         error_ = "error on child read ";
         error_ += e.what();
      } // end try/catch

      // if exception occured throw  
      if(error_.length() > 0) throw error_.c_str();

      return ret;
   } // end Get1dData

   /// \brief template function to read a scalar from the property tree
   template<typename T>
   T GetScalarData(pt::ptree tree, const string &child_label) {
      T ret;
      string error;

      try {
         ret = tree.get<T>(child_label);
      } catch(std::exception &e) {
         error_ = "error on child read ";
         error_ += e.what();
      } // end try/catch

      // if exception occured throw  
      if(error_.length() > 0) throw error_.c_str();

      return ret;
   } // end GetScalarData

}; // end class 

#endif  // end header guard

