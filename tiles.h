#ifndef TILES_H
#define TILES_H

#include <string>

enum TileType
{
	// VISIBLE TILES
	VOID	= 1 << 0,
	WALL	= 1 << 1,
	FLOOR	= 1 << 2,
	GRASS	= 1 << 3,
	ROAD	= 1 << 4,
	
	// WHAT THE TILE IS A PART OF
	ROOM	= 1 << 5,
	HALLWAY = 1 << 6,

	// OTHER FLAGS
	PORTAL_UNLINKED = 1 << 7,
	PORTAL_LINKED	= 1 << 8,
	PLAYER_SPAWN	= 1 << 9
};

struct Tile
{
	TileType type;
	std::string symbol;
	std::string color;
	bool discovered;

	Tile(TileType t);
	void changeType(TileType newType);

private:
	// Determine the symbol and color based on the tile's type
	void evaluateType();
};

#endif
