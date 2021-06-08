# gdoor
A raspberry pi zero project that:
1) uses the gpio lib to read digital inputs on some loop rate
2) write the door status to a sqlite3 database/table (garage/door_state)
3) use php in apache to display the status on a network accessable webpage

This project is just to learn how to do raspberry pi projects. The program is written in c++, since that what I know. And a c++ application is ~16 times faster than a python script. 

The raspberry pi zero w/ a 16G sd card can run standard programs and this project will push some assumptions on what I can build and run with moderate performance. 


**Features**
* monitor and record the garage door open/close state 
  * present as a table in a web page
* monitor and record the temperature and humidity inside the garage
  * present as a 24hr chart on a web page

**Hardware**
*Wiring*
**Software Libraries**
**Software Structure**

ref: https://guides.github.com/features/mastering-markdown/

__I2C__ Si7021 temperaure and humidity sensor
 name | color | pin 
---|---|---
 3.3v | red   |  1 
 0v   | brn   |  6 
 SDA  | yel   | 3 
 SCL  | org   | 5 

__Door cycling LED__ An indicator to know that the software is running
 name | color | pin 
---|---|---
0v | blk | 39
gpio27 | yel | 36

__Up/down switches__ Two switch inputs for the door position
 name | color | pin 
---|---|---
3.3v | red | 17
0v | blk | 25
up | gpio23 | org | 33
down | gpio24 | yel| 35

____typical circuit for the switch inputs____
![Switch input circuit](/drawings/switch_input.jpg)


__Help light__ (stacked connector (relay hat), so 5/3.3V and 0v not known)
 name | color | pin 
---|---|---
gpio25 | na | 37
The light is then connected with L1 to relay C1 and NO1

![State Machine UML](/drawings/uml_state.jpg)

The project uses the boost::ext sml state machine library. ref: https://boost-ext.github.io/sml/index.html. This is a good project to learn the library since the door sequencing is simple. The actual transition table is shown here. 

```C++
// garage door sequencing 
state<Idle1> + event<eGdInit> / SetMovingLedOff = state<Startup>,
state<Startup> + event<eOnTime>[(IsUp && !IsDown)] / [] {} = state<Open>,
state<Startup> + event<eOnTime>[(!IsUp && IsDown)] / [] {} = state<Closed>,
state<Open> + sml::on_entry<_> / AddRowForOpen,
state<Closed> + sml::on_entry<_> / AddRowForClosed,
state<Open> + event<eOnTime>[!IsUp] / SetMovingLedOn = state<MovingToClose>,
state<Closed> + event<eOnTime>[!IsDown] / SetMovingLedOn =state<MovingToOpen>,
state<MovingToClose> + sml::on_entry<_> / AddRowForMovingToClose,
state<MovingToOpen> + sml::on_entry<_> / AddRowForMovingToOpen,
state<MovingToClose> + event<eOnTime>[IsDown] / SetMovingLedOff =state<Closed>,
state<MovingToClose> + event<eOnTime>[IsUp] / SetMovingLedOff = state<Open>,
state<MovingToOpen> + event<eOnTime>[IsUp] / SetMovingLedOff = state<Open>,
state<MovingToOpen> + event<eOnTime>[IsDown] / SetMovingLedOff =state<Closed>

```

