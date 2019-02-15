#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <string>
namespace Symbols
{
	/* PLAYER */
	// Player - @ - Cyan
	const std::string ENT_PLAYER	= "@";

	/* WORLD TILES */
	const std::string TILE_VOID	= " ";
	const std::string TILE_WALL	= "#";
	const std::string TILE_FLOOR	= ".";
	const std::string TILE_GRASS	= ",";
	const std::string TILE_ROAD	= "~";
};

namespace SymbolColors
{
	// DEFAULT COLOR
	#define DEFAULT "\033[0m"

	/* PLAYER */
	const std::string ENT_PLAYER = "\033[0;36m"; // Cyan

	/* WORLD TILES */
	const std::string TILE_VOID	= DEFAULT;
	const std::string TILE_WALL	= "\033[0;33m";
	const std::string TILE_FLOOR	= DEFAULT;
	const std::string TILE_GRASS	= "\033[0;32m"; // Green
	const std::string TILE_ROAD	= DEFAULT;
};
#endif
