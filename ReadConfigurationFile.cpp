/// file: ReadConfigurationFile.cpp Implementation for ReadConfigurationFile class  
/// class: CS6030 Bioinfomatics Algorithms
/// assignment: Project, Hidden Markov Models  
/// author: Bennett Cook
/// email: bennett.j.cook@wmich.edu
/// date: 11-21-2019
/// description: 

#include "ReadConfigurationFile.h"

/// \brief constructor
ReadConfigurationFile::ReadConfigurationFile() {
} // end ctor 

/// \brief destructor
ReadConfigurationFile::~ReadConfigurationFile() {
} // end dtor 

/// \brief perform the jsone file read-in and convert element to 
/// HmmConfiguration members.
int ReadConfigurationFile::ReadIn() {
   int ret = 0;

   pt::ptree tree;

   try {
      read_json(configFilename_, tree);

      hmmConfig_.A = Get2dData<long double>(tree, CONFIG_ID_A);
      hmmConfig_.A_labels = Get1dData<string>(tree, CONFIG_ID_A_LABELS);
      hmmConfig_.B = Get2dData<long double>(tree, CONFIG_ID_B);
      hmmConfig_.O_labels = Get1dData<string>(tree, CONFIG_ID_O_LABELS);
      hmmConfig_.pi = Get1dData<long double>(tree, CONFIG_ID_PI);
      hmmConfig_.name = GetScalarData<string>(tree, CONFIG_ID_NAME);
      hmmConfig_.type = GetScalarData<string>(tree, CONFIG_ID_TYPE);
      hmmConfig_.desc = GetScalarData<string>(tree, CONFIG_ID_DESC);

      // observations are in an array of strings, concatenated 
      // into hmmConfig_.O
      vector<string> obs_lines;
      obs_lines = Get1dData<string>(tree, CONFIG_ID_OBSERVATIONS);
      for(string line : obs_lines)
         hmmConfig_.O += line;

      // test if CONFIG_ID_TRAINING is in the tree
      boost::optional<string> tmp1 = tree.get_optional<string>(CONFIG_ID_TRAINING);
      if(tmp1.is_initialized()) {

         // training lines are an array of strings, concatenated 
         // into hmmConfig_.training
         vector<string> training_lines;
         training_lines = Get1dData<string>(tree, CONFIG_ID_TRAINING);
         for(string line : training_lines)
            hmmConfig_.training += line;

         boost::optional<string> tmp1 = tree.get_optional<string>(CONFIG_ID_MAX_ITERATIONS);
         if(tmp1.is_initialized()) {
            hmmConfig_.maxIterations = lexical_cast<int>(GetScalarData<string>(tree, CONFIG_ID_MAX_ITERATIONS));
         }
         else {
            hmmConfig_.maxIterations = MAX_ITERATIONS;
         } // end if 

      } // end if 
   }
   catch(std::exception &e) {
      error_ = "error on read ";
      error_ += e.what();
      ret = -1;
   } // end try/catch

   return ret;
} // end ReadIn

