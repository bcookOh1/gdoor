
// header guard
#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

// ref: https://boost-ext.github.io/sml/index.html
// https://boost-ext.github.io/sml/examples.html, hello world
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <chrono>
#include <cassert>
#include <vector>
#include <boost/sml.hpp>
#include <boost/mpl/placeholders.hpp>

#include "CommonDef.h"
#include "Util.h"
#include "UpdateDatabase.h"

using namespace std;
namespace sml = boost::sml;


// anonymous namespace to hide definitions
namespace {

// events 
struct eGdInit {};
struct eOnTime {};

// for this implementation, states are just empty classes
class Idle1;
class Startup;
class MovingToOpen;
class Open;
class MovingToClose;
class Closed;

// transition  table
// example: src_state +event<>[guard] / action = dst_state,
// transition from src_state to dst_state on event with guard and action
class sm_garage_door {
   using self = sm_garage_door;
public:

   explicit sm_garage_door(IoValues &ioValues, UpdateDatabase &udb, string &temp) :
      _ioValues(ioValues), _udb(udb), _temperature(temp) {
   } // end ctor 

   // function called from action lambdas
   void AddDoorStateRow(DoorState ds) {
      cout << "AddDoorStateRow state: " << (int)ds << endl;
      UpdateDoorStateDB(ds, _udb, _temperature);
   } // end AddDoorStateRow


   auto operator()() const {
      using namespace sml;

      // guards
      auto IsUp = [this] () -> bool {
         // cout << "IsUp" << endl;
         return (_ioValues["up_position"] == 1);
      }; // end IsUp

      auto IsDown = [this] () -> bool {
         // cout << "IsDown" << endl;
         return (_ioValues["down_position"] == 1);
      }; // end IsDown


      // actions for updating the database
      auto AddRowForOpen = [] (self s) {
         s.AddDoorStateRow(DoorState::Open);
      }; // AddRowForOpen

      auto AddRowForClosed = [](self s) {
         s.AddDoorStateRow(DoorState::Closed);
      }; // AddRowForClosed

      auto AddRowForMovingToOpen = [](self s) {
         s.AddDoorStateRow(DoorState::MovingToOpen);
      }; // AddRowForMovingToOpen

      auto AddRowForMovingToClose = [](self s) {
         s.AddDoorStateRow(DoorState::MovingToClose);
      }; // AddRowForMovingToClose


      auto SetMovingLedOn = [this] { 
         // cout << "SetMovingLedOn" << endl;
         _ioValues["door_cycling"] = 1; 
      };

      auto SetMovingLedOff = [this] { 
         // cout << "SetMovingLedOff" << endl;
         _ioValues["door_cycling"] = 0; 
      };

      return make_transition_table(

         /* garage door sequencing */
         *state<Idle1> + event<eGdInit> / SetMovingLedOff = state<Startup>,

          state<Startup> + event<eOnTime>[(IsUp && !IsDown)] / [] {} = state<Open>,
          state<Startup> + event<eOnTime>[(!IsUp && IsDown)] / [] {} = state<Closed>,

          state<Open> + sml::on_entry<_> / AddRowForOpen,
          state<Closed> + sml::on_entry<_> / AddRowForClosed,

          state<Open> + event<eOnTime>[!IsUp] / SetMovingLedOn = state<MovingToClose>,
          state<Closed> + event<eOnTime>[!IsDown] / SetMovingLedOn = state<MovingToOpen>,

          state<MovingToClose> + sml::on_entry<_> / AddRowForMovingToClose,
          state<MovingToOpen> + sml::on_entry<_> / AddRowForMovingToOpen,

          state<MovingToClose> + event<eOnTime>[IsDown] / SetMovingLedOff = state<Closed>,
          state<MovingToClose> + event<eOnTime>[IsUp] / SetMovingLedOff = state<Open>,

          state<MovingToOpen> + event<eOnTime>[IsUp] / SetMovingLedOff = state<Open>,
          state<MovingToOpen> + event<eOnTime>[IsDown] / SetMovingLedOff = state<Closed>
         
      );
   } // end operator() 

private:

   // digital io data member 
   IoValues &_ioValues;
   UpdateDatabase &_udb;

   // pi temperature 
   string &_temperature;

}; // end struct

} // end anonymous namespace

#endif // end header guard
