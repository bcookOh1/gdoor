# gdoor
## Introduction 
A raspberry pi zero project that:
1) uses the gpio lib to read digital inputs on some loop rate
2) write the door status to a sqlite3 database/table (garage/door_state)
3) use php in apache to display the status on a network accessable webpage

This project is just to learn how to do raspberry pi projects. The program is written in c++, since that what I know. And a c++ application is ~16 times faster than a python script. 

The raspberry pi zero w/ a 16G sd card can run standard programs and this project will push some assumptions on what I can build and run with moderate performance. 

**Features**
* monitor and record the garage door open/close state 
  * present as a table on the web 
* includes a "help light" turned on for N minutes triggered with the door opens/closes
* monitor and record the temperature and humidity inside the garage
  * use a Si7021 sensor read using I2C
  * present readings as a 24hr chart on the web page
* runs as a singleton to prevent I/O set from multiple programs
  * if one instance of gdoor detects another instance it exits
* uses a json configuration file for:
  * I/O name and pin assignments
  * timers or timebase values   

**Hardware**
1. Raspberry PI Zero W w/ 5v 2amp power supply
2. Si7021 sensor
3. RPi Power Relay Board
4. mechanical switches and wire for door up/down position
5. light fixture and ac power cord 

**Software packages and libraries**
1. Raspbian
2. Apache
3. Php
4. Sqlite3
5. Gnuplot 
6. PiWiring
7. Boost C++
8. Boost-ext sml 

All are the lastest versions. 

**Software structure**
Gdoor uses a while loop as the main control structure following these steps:
1. read inputs
2. run sequencing code 
3. set outputs
4. repeat
right, this is only a polling structure 

**The configuration file**
This configuration file is in json format, which is easy to write and edit. I like this configuration format must better that xml. The program uses Boost.PropertyTree to read and parse the file. The PropertyTree library is very easy to use and is a good tool for hierarchical and record based data.

The config file is shown here: 
```
{
  "GarageDoorPi": {
    "name": "garage door monitor",
    "database_path": "/home/bjc/gdoor/exe/garage.db",
    "door_state_table": "door_state",
    "sensor_table": "readings",
    "digital_io": [
       { 
         "type": "input",
         "name": "up_position", 
         "pin": 23
       },
       { 
         "type": "input",
         "name": "down_position", 
         "pin": 24
       },
       { 
         "type": "output",
         "name": "door_cycling", 
         "pin": 27
       },
       { 
         "type": "output",
         "name": "help_light", 
         "pin": 25
       } 
    ],
    "loop_time_ms": 500,
    "pi_temp_read_interval_sec": 30, 
    "sensor_read_interval_sec": 300, 
    "help_light_on_time_sec": 600 
  }
}
```

ref: https://guides.github.com/features/mastering-markdown/

__GPIO phyical wiring__

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


__Help light__ (stacked connector (relay hat), so the 5/3.3V and 0v pins are not known)
 name | color | pin 
---|---|---
gpio25 | na | 37
The light is then connected with L1 to relay C1 and NO1

![State Machine UML](/drawings/uml_state.jpg)

The project uses the boost::ext sml state machine library. ref: https://boost-ext.github.io/sml/index.html. This is a good project to learn the library since the door sequencing is simple. The transition table is shown here. 

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

```
pi@rpz01:~ $ gpio readall
 +-----+-----+---------+------+---+-Pi ZeroW-+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 0 | IN   | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | IN   | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | OUT  | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+-Pi ZeroW-+---+------+---------+-----+-----+
```
 
__Database__ 
 garage.db
sqlite> .tables
door_state  readings

sqlite> .schema door_state
CREATE TABLE door_state (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  rec_time text not null,
  state int not null,
  temperature text not null
);

sqlite> .schema readings
CREATE TABLE readings (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  timestamp text not null,
  temperature text not null,
  temperature_units text not null,
  humidity text not null,
  humidity_units text not null
);

both gdoor and index.php use simple sqlite3_exec() and $db->query()  

__Webpage__
http://www.rpz01.gleeze.com/

__Program startup__
_from the command line_ 

usage: ./gdoor [-h] -c <config_file> [-s] 
  -h, optional, shows this help text, if included other arguments are ignored and the program shows this text
  -c <config_file>, a json file with the configuration 
  -s, optional, the io and state information is not printed but error messages are 
Note: a space is required between -c and the config file 
example: ./gdoor -c config_1.json -s 

_as a service_ 

/usr/local/bin

__Some interesting classes__
- sml: used for the state machine logic. see above 
- OneShot: the Changed() member outputs true once each time the value under test changes. This is useful in the while-loop controller when a digital input changes changes state and you need to set some action once when the change occurs. The idea comes from the basic digital electronics circuit call a one shot.  
- Reader: This is a base class active object. It is used in the while-loop controller when a feature/reading happens on a timed interval. The time-base has one second resolution. It's used for these three features: reading the Pi Temperature, reading the Si7021 temperature and humidity, and setting the help light.  
