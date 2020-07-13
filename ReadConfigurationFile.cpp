/// file: ReadConfigurationFile.cpp Implementation for ReadConfigurationFile class  
/// author: Bennett Cook
/// date: 07-08-2020
/// description: 

#include "ReadConfigurationFile.h"

/// \brief constructor
ReadConfigurationFile::ReadConfigurationFile() {
} // end ctor 

/// \brief destructor
ReadConfigurationFile::~ReadConfigurationFile() {
} // end dtor 

/// \brief perform the json file read-in and convert element to 
/// AppConfig members.
int ReadConfigurationFile::ReadIn() {
   int ret = 0;

   pt::ptree tree;

   try {
      read_json(_configFilename, tree);

      _appConfig.appName = GetScalarData<string>(tree, CONFIG_APP_NAME);
      _appConfig.dbPath = GetScalarData<string>(tree, CONFIG_DB_PATH);
      _appConfig.dbTable = GetScalarData<string>(tree, CONFIG_DB_TABLE);

     //  _appConfig.dInputs = Get1dData<unsigned>(tree, CONFIG_DIGITAL_INPUTS);
     pt::ptree inputsTree = tree.get_child(CONFIG_DIGITAL_INPUTS);


     //  _appConfig.dInputLabels = Get1dData<string>(tree, CONFIG_DIGITAL_INPUT_LABELS);
      _appConfig.loopTimeMS = GetScalarData<int>(tree, CONFIG_LOOP_TIME_MS);

   }
   catch(std::exception &e) {
      _statusStr = "error on read ";
      _statusStr += e.what();
      ret = -1;
   } // end try/catch

   return ret;
} // end ReadIn

