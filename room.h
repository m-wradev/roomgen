#ifndef ROOM_H
#define ROOM_H

// Room is a builder class; it will not be instantiated
#include <vector>
#include <map>
#include "tiles.h"

class Room
{
private:
	static const int NUMBER_LAYOUTS = 2;

	enum RoomLayout
	{
		SIMPLE_BOX	= 0,
		CIRCLE		= 1,
	};

	static void buildSimpleBox(std::vector< std::vector<TileType> > *roomdata);
	static void buildCircle(std::vector< std::vector<TileType> > *roomdata);

public:
	static std::vector< std::vector<TileType> > buildRoom();
	static void generateHallway(unsigned long x, unsigned long y, std::map<unsigned long, std::map<unsigned long, Tile> > *tiles);
};

#endif
