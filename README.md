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
 
