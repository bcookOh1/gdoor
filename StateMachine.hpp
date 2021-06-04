
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

// guards
auto IsUp = [](IoValues &ioValues)-> bool {
   return (ioValues["up_position"] == 1);
}; // end IsUp

auto IsDown = [](IoValues &ioValues) -> bool {
   return (ioValues["down_position"] == 1);
}; // end IsDown


// states are just classes, use parent with virtual GetDoorState() to get individual states
// when setting the db record
// garage door states
class DoorStateBase { public: virtual DoorState GetDoorState(){return DoorState::NoChange;} };
class Idle1 : public DoorStateBase { public: DoorState GetDoorState(){return DoorState::NoChange;} };
class Startup : public DoorStateBase { public: DoorState GetDoorState(){return DoorState::Startup;} };
class MovingToOpen : public DoorStateBase { public: DoorState GetDoorState(){return DoorState::MovingToOpen;} };
class Open : public DoorStateBase { public: DoorState GetDoorState(){return DoorState::Open;} };
class MovingToClose : public DoorStateBase { public: DoorState GetDoorState(){return DoorState::MovingToClose;} };
class Closed : public DoorStateBase { public: DoorState GetDoorState(){return DoorState::Closed;} };

// transition  table
// example: src_state +event<>[guard] / action = dst_state,
// transition from src_state to dst_state on event with guard and action
class sm_garage_door {
   using Self = sm_garage_door;
public:

   explicit sm_garage_door(IoValues &ioValues, UpdateDatabase &udb, string &temp) :
      _ioValues(ioValues), _udb(udb), _temperature(temp) {
   } // end ctor 


   auto operator()() const {
      using namespace sml;

      // guards
      auto IsUp = [this] () -> bool {
         return (_ioValues["up_position"] == 1);
      }; // end IsUp

      auto IsDown = [this] () -> bool {
         return (_ioValues["down_position"] == 1);
      }; // end IsDown


         // actions
      auto AddDoorStateRow = [&,this] (DoorStateBase dsb) { 
         UpdateDoorStateDB(dsb.GetDoorState(), _udb, _temperature); 
      };

      auto SetMovingLedOn = [this] { 
         _ioValues["door_cycling"] = 1; 
      };

      auto SetMovingLedOff = [this] { 
         _ioValues["door_cycling"] = 0; 
      };

      return make_transition_table(

         /* garage door sequencing */
         *state<Idle1> + event<eGdInit> / SetMovingLedOff = state<Startup>,

          state<Startup> + event<eOnTime>[(IsUp && !IsDown)] / [] {} = state<Open>,
          state<Startup> + event<eOnTime>[(!IsUp && IsDown)] / [] {} = state<Closed>,

          state<Open> + sml::on_entry<_> / AddDoorStateRow,
          state<Closed> + sml::on_entry<_> / AddDoorStateRow,

          state<Open> + event<eOnTime>[!IsUp] / SetMovingLedOn = state<MovingToClose>,
          state<Closed> + event<eOnTime>[!IsDown] / SetMovingLedOn = state<MovingToOpen>,
 
          state<MovingToClose> + sml::on_entry<_> / AddDoorStateRow,
          state<MovingToOpen> + sml::on_entry<_> / AddDoorStateRow,

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

// use this "public" alias for the state machine for 
// use in another file 
// using Gdsm = sm_garage_door;

#endif // end header guard
