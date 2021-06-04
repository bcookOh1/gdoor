# Raspberry pi garage door project
# Bennett Cook

# compiler for c++
CC = g++

# compiler flags, all warnings and use c++2a libraries
CFLAGS = -Wall -std=c++2a 

LFLAGS = -L/usr/lib/arm-linux-gnueabihf -lsqlite3 -lwiringPi -lpthread -lboost_system -lboost_filesystem

# the executable to build
TARGET = gdoor

all: $(TARGET)

# set cpp macro to display debug cout lines 
debug: CC += -g
debug: $(TARGET)

$(TARGET): main.o UpdateDatabase.o ReadConfigurationFile.o DigitalIO.o DoorSensor.o ParseCommandLine.o Util.o ProcessCount.o Si7021.o I2C.o Reader.o HelpLightReader.o PiTempReader.o Si7021Reader.o
	$(CC) $(LFLAGS) -o $(TARGET)  main.o UpdateDatabase.o ReadConfigurationFile.o DigitalIO.o DoorSensor.o ParseCommandLine.o Util.o ProcessCount.o Si7021.o I2C.o Reader.o HelpLightReader.o PiTempReader.o Si7021Reader.o

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp 

UpdateDatabase.o: UpdateDatabase.cpp UpdateDatabase.h 
	$(CC) $(CFLAGS) -c UpdateDatabase.cpp

ReadConfigurationFile.o: ReadConfigurationFile.cpp ReadConfigurationFile.h 
	$(CC) $(CFLAGS) -c ReadConfigurationFile.cpp

DigitalIO.o: DigitalIO.cpp DigitalIO.h 
	$(CC) $(CFLAGS) -c DigitalIO.cpp

DoorSensor.o: DoorSensor.cpp DoorSensor.h 
	$(CC) $(CFLAGS) -c DoorSensor.cpp

Util.o: Util.cpp Util.h  
	$(CC) $(CFLAGS) -c Util.cpp

ParseCommandLine.o: ParseCommandLine.cpp ParseCommandLine.h  
	$(CC) $(CFLAGS) -c ParseCommandLine.cpp

ProcessCount.o: ProcessCount.cpp ProcessCount.h  
	$(CC) $(CFLAGS) -c ProcessCount.cpp

Si7021.o: Si7021.cpp Si7021.h 
	$(CC) $(CFLAGS) -c Si7021.cpp

I2C.o: I2C.cpp I2C.h 
	$(CC) $(CFLAGS) -c I2C.cpp

Reader.o: Reader.cpp Reader.h  
	$(CC) $(CFLAGS) -c Reader.cpp

HelpLightReader.o: HelpLightReader.cpp HelpLightReader.h  
	$(CC) $(CFLAGS) -c HelpLightReader.cpp

PiTempReader.o: PiTempReader.cpp PiTempReader.h  
	$(CC) $(CFLAGS) -c PiTempReader.cpp

Si7021Reader.o: Si7021Reader.cpp Si7021Reader.h  
	$(CC) $(CFLAGS) -c Si7021Reader.cpp


clean:  
	$(RM) $(TARGET) 
	$(RM) *.o 
	$(RM) *.gch 
