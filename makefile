# Raspberry pi garage door project
# Bennett Cook

# compiler for c++
CC = g++

# compiler flags, all warnings and use c++17 libraries
CFLAGS = -Wall -std=c++17

LFLAGS = -lsqlite3

# the executable to build
TARGET = gdoor.out

all: $(TARGET)

# set cpp macro to display debug cout lines 
# debug: CC += -D_DEBUG_STATEMENTS
# debug: $(TARGET)

$(TARGET): main.o UpdateDatabase.o ReadConfigurationFile.o Util.o
	$(CC) $(LFLAGS) -o $(TARGET)  main.o UpdateDatabase.o ReadConfigurationFile.o Util.o

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp 

UpdateDatabase.o: UpdateDatabase.cpp UpdateDatabase.h 
	$(CC) $(CFLAGS) -c UpdateDatabase.cpp

ReadConfigurationFile.o: ReadConfigurationFile.cpp ReadConfigurationFile.h 
	$(CC) $(CFLAGS) -c ReadConfigurationFile.cpp

Util.o: Util.cpp Util.h  
	$(CC) $(CFLAGS) -c Util.cpp Util.h

clean:  
	$(RM) $(TARGET) 
	$(RM) *.o 
	$(RM) *.gch 
