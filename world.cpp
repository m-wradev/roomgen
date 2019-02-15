#include "world.h"

#include <stdlib.h>
#include <unordered_set>

// VARIABLES
unsigned long World::pSpawn_x;
unsigned long World::pSpawn_y;

std::vector<Room> World::rooms;

std::vector< std::vector<TileType> > World::createWorld()
{
	std::vector< std::vector<TileType> > world;
	setFoundation(&world);
	carveRooms(&world);
	connectRooms(&world);
	setPlayerSpawn(&world);

	return world;
}

void World::generateGraph(int worldSize)
{
	Graph graph;
	graph.nodes = generateNodes(worldSize);
	graph.paths = generatePaths(graph.nodes);
}

std::vector<GraphNode> World::generateNodes(int worldSize)
{
	
}

std::vector<GraphPath> World::generatePaths(std::vector<GraphNode> nodes)
{
	// there should be no loops
	// every node should be connected to at least one other node
	#define MAX_CONNECTIONS 2
	std::vector<GraphPath> paths;

	for (GraphNode node : nodes)
	{
		std::unordered_set<GraphNode> connectedNodes;

		// every node should have at most 2 connections
		int connections = rand() % MAX_CONNECTIONS + 1;
		for (int i = 0; i < connections; i++)
		{
			GraphNode *toConnect;
			// keep pulling nodes from the list of nodes if a path would create a loop or we're creating an equivalent path
			for (toConnect = &nodes[rand() % 0 + nodes.Size()]; toConnect == &node || !connectedNodes.insert(connected).second ;)
				toConnect = &nodes[rand() % 0 + nodes.size()];

			GraphPath path;
			path.a = &node;
			path.b = toConnect;

			paths.push_back(path);
		}
	}

	return paths;
}

void World::setFoundation(std::vector< std::vector<TileType> > *world)
{
	// We'll only make maps that are 64 * 64 right now
	for (int r = 0; r < 64; r++)
	{
		std::vector<TileType> row;
		world->push_back(row);
		for (int c = 0; c < 64; c++)
		{
			(*world).at(r).push_back(WALL);
		}
	}
}

void World::carveRooms(std::vector< std::vector<TileType> > *world)
{
	// we'll do five rooms for now
	int numRooms = 10;
	for (int i = 0; i < numRooms; i++)
	{
		// We'll only make boxes for now
		// Our rooms will only be between
		// l = 5 and l = 10
		// w = 5 and w = 10
		// for now
		Room r;
		r.l = rand() % 10 + 5; r.l = (r.l % 2 == 0) ? r.l + 1 : r.l;
		r.w = rand() % 10 + 5; r.w = (r.w % 2 == 0) ? r.w + 1 : r.w;

		// Find a place on the map where the room will fit
		bool placementSuccess = false;
		while (!placementSuccess)
		{
			// The coordinates are backward since we 
			// inserted rows first, then columns
			r.x = rand() % world->size();
			r.y = rand() % world->at(0).size();

			// We don't want to place a room where it can't be fully
			// traversed.
			// Rooms should not be placed so that they intersect
			// other rooms.  They should not be placed so they are
			// partially cut off by the boundaries of the world.
			if	( validPlacement(&r, world) )
			{
				placeRoom(&r, world);
				rooms.push_back(r);
				placementSuccess = true;
			}
		}
	}
}

// HELPER
// FOR: carveRooms()
// WHAT: Check to make sure that a potential room is within the bounds of the map and doesn't
//	 intersect another roomi
// RETURN: true if the location is a valid placement, false otherwise
bool World::validPlacement(Room *r, std::vector< std::vector<TileType> > *world)
{
	// First make sure that the room doesn't go out of bounds of the world
	if	(  (r->x - r->w/2 < 0 || r->x + r->w/2 >= world->at(0).size())
		|| (r->y - r->l/2 < 0 || r->y + r->l/2 >= world->size()) )
		return false;

	// Now check for intersection of other rooms
	for (int row = r->y - r->l/2; row <= r->y + r->l/2; row++)
	{
		for (int col = r->x - r->w/2; col <= r->x + r->w/2; col++)
		{
			if ((*world).at(row).at(col) == FLOOR) return false;
		}
	}

	return true;
}

// HELPER
// FOR: carveRooms()
// WHAT: Place a room at a certain location
// RETURN: void
void World::placeRoom(Room *r, std::vector< std::vector<TileType> > *world)
{
	for (int row = r->y - r->l/2; row <= r->y + r->l/2; row++)
	{
		for (int col = r->x - r->w/2; col <= r->x + r->w/2; col++)
		{
			(*world).at(row).at(col) = FLOOR;
		}
	}
}

void World::connectRooms(std::vector< std::vector<TileType> > *world) 
{
	// 70% chance to connect to a nearby room
	// 30% chance to connect to a far away room
}

void World::setPlayerSpawn(std::vector< std::vector<TileType> > *world)
{
	// This is just arbitrary right now
	int room = rand() % World::rooms.size();
	TileType cur = world->at(World::rooms[room].y).at(World::rooms[room].x);
	world->at(World::rooms[room].y).at(World::rooms[room].x) = (TileType)(cur | PLAYER_SPAWN);

	pSpawn_x = World::rooms[room].x;
	pSpawn_y = World::rooms[room].y;
}
