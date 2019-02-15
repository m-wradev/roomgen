#include "tiles.h"

#include "symbols.h"

#include <stdio.h>

Tile::Tile(TileType t)
{
	type = t;
	evaluateType();

	discovered = false;
}

void Tile::changeType(TileType newType)
{
	type = newType;
	evaluateType();
}

void Tile::evaluateType()
{
	// Set symbols and colors
	if	(type == VOID)
	{
		symbol = Symbols::TILE_VOID;
		color  = SymbolColors::TILE_VOID;
	}
	else if (type & WALL)
	{
		symbol = Symbols::TILE_WALL;
		color  = SymbolColors::TILE_WALL;
	}
	else if (type & FLOOR)
	{
		symbol = Symbols::TILE_FLOOR;
		color  = SymbolColors::TILE_FLOOR;
	}
	else if (type & GRASS)
	{
		symbol = Symbols::TILE_GRASS;
		color  = SymbolColors::TILE_GRASS;
	}
	else if (type & ROAD)
	{
		symbol = Symbols::TILE_ROAD;
		color  = SymbolColors::TILE_ROAD;
	}
}
