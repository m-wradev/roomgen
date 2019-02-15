#default: roomgen2.cpp room.o tiles.o
#	g++ -std=c++11 -g -o roomgen2 tiles.o room.o roomgen2.cpp // Deprecated

default: roomgen2.cpp world.o tiles.o
	g++ -std=c++11 -g -o roomgen2 tiles.o world.o roomgen2.cpp

#room: room.cpp
#	g++ -std=c++11 -o room.o -c room.cpp

world: world.cpp
	g++ -std=c++11 -g -o world.o -c world.cpp

tiles: tiles.cpp
	g++ -std=c++11 -g -o tiles.o -c tiles.cpp

#all: roomgen2.cpp room.cpp tiles.cpp
#	g++ -std=c++11 -g -o tiles.o -c tiles.cpp
#	g++ -std=c++11 -g -o room.o -c room.cpp
#	g++ -std=c++11 -g -o roomgen2 roomgen2.cpp room.o tiles.o

all: roomgen2.cpp world.cpp tiles.cpp
	g++ -std=c++11 -g -o tiles.o -c tiles.cpp
	g++ -std=c++11 -g -o world.o -c world.cpp
	g++ -std=c++11 -g -o roomgen2 roomgen2.cpp world.o tiles.o
