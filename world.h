#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "tiles.h"

struct Room
{
	unsigned int x, y; // Defines center of room
	char l, w; // length and width of room
	char r;	// Radius of room (if required by shape)
};

struct GraphNode
{
	int x, y;
};

// paths are not directed
struct GraphPath
{
	GraphNode *a;
	GraphNode *b;
	int weight;
};

struct Graph
{
	std::vector<GraphNode> nodes;
	std::vector<GraphPath> paths;
};


class World
{
private:
	enum RoomType
	{
		BOX	= 1 << 0
	};
		
	static std::vector<Room> rooms;

	// Main generation procedure
	static void setFoundation(std::vector< std::vector<TileType> > *world);
	static void carveRooms(std::vector< std::vector<TileType> > *world);
	static void connectRooms(std::vector< std::vector<TileType> > *world);
	static void setPlayerSpawn(std::vector< std::vector<TileType> > *world);

	static void generateGraph(int worldSize);
	static std::vector<GraphNode> generateNodes(int worldSize);
	static std::vector<GraphPath> generatePaths(std::vector<GraphNode> nodes);

	// Helper methods
	static bool validPlacement(Room *r, std::vector< std::vector<TileType> > *world);
	static void placeRoom(Room *r, std::vector< std::vector<TileType> > *world);

public:
	static std::vector< std::vector<TileType> > createWorld();
	static unsigned long pSpawn_x;
	static unsigned long pSpawn_y;
};

#endif //WORLD_H
