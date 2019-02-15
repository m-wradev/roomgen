#include <stdlib.h>
#include <time.h>
#include <utility>
#include "room.h"

std::vector< std::vector<TileType> > Room::buildRoom()
{
	// Initialize random seed
	srand(time(NULL));
	std::vector< std::vector<TileType> > roomdata;
	int roomLayout = rand() % NUMBER_LAYOUTS;
	switch (roomLayout)
	{
		case SIMPLE_BOX:			
			buildSimpleBox(&roomdata);
			break;
		case CIRCLE:
			buildCircle(&roomdata);
			break;
	}

	return roomdata;
}


void Room::buildSimpleBox(std::vector< std::vector<TileType> > *roomdata)
{
	static const int MIN_HEIGHT = 6;
	static const int MAX_HEIGHT = 14;
	static const int MIN_WIDTH  = 6;
	static const int MAX_WIDTH  = 14;

	static const int NUM_DOOR_LAYOUTS = 17;
	enum DoorLayouts
	{
		/*
		 * CODES
		 * M = Middle of wall (single door)
		 * S = Same wall (dual)
		 *
		 * U = up, D = down
		 * R = right, L = left
		 */

		// ONE: MID  		// TWO: SAME	// FOUR: MID
		ONE_M_U	    = 0,	TWO_S_U	 = 8,	FOUR_M_ALL = 16,
		ONE_M_R	    = 1,	TWO_S_R	 = 9,
		ONE_M_D	    = 2,	TWO_S_D	 = 10,
		ONE_M_L	    = 3,	TWO_S_L	 = 11,
		
		// THREE: MID		// TWO: MID
		THREE_M_URD = 4,	TWO_M_UR = 12,
		THREE_M_RDL = 5,	TWO_M_RD = 13,
		THREE_M_DLU = 6,	TWO_M_DL = 14,
		THREE_M_LUR = 7,	TWO_M_LU = 15
	};

	// Generate the height and width of the room
	int height = rand() % MAX_HEIGHT + MIN_HEIGHT;
	height = (height % 2 == 0) ? height + 1 : height; // Always make the height odd

	int width  = rand() % MAX_WIDTH  + MIN_WIDTH;
	width = (width % 2 == 0) ? width + 1 : width; // Always make the width odd
		
	// Build the room
	for (int r = 0; r < height; r++)
	{
		std::vector<TileType> temp;
		roomdata->push_back(temp);
		for (int c = 0; c < width; c++)
		{
			// The edges of the room will always be a wall tile unless we're generating a door
			// then it will be a floor or door tile
			if ( (r == 0 || r == height - 1) || (c == 0 || c == width - 1) )
			{	
				roomdata->at(r).push_back(WALL);
			}
			else
			{
				roomdata->at(r).push_back(FLOOR);
			}
		}
	}

	// Set doors and portals.  This will get messy
	// Generate a random number to pick a layout
	int layout = rand() % NUM_DOOR_LAYOUTS;

	if	(layout == ONE_M_U)	{ roomdata->at(0)[width/2]			= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == ONE_M_R)	{ roomdata->at(height/2)[width-1] 		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == ONE_M_D)	{ roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == ONE_M_L)	{ roomdata->at(height/2)[0]			= (TileType)(FLOOR | PORTAL_UNLINKED);  }

	else if (layout == TWO_S_U)	{ roomdata->at(0)[width/3-1]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(0)[width-width/3]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == TWO_S_R)	{ roomdata->at(height/3-1)[width-1] 		= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(height-height/3)[width-1]	= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == TWO_S_D)	{ roomdata->at(height-1)[width/3-1]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(height-1)[width-width/3]		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == TWO_S_L)	{ roomdata->at(height/3-1)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(height-height/3)[0]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }

	else if (layout == TWO_M_UR)	{ roomdata->at(0)[width/2]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(height/2)[width-1]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == TWO_M_RD)	{ roomdata->at(height/2)[width-1]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == TWO_M_DL)	{ roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(height/2)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == TWO_M_LU)	{ roomdata->at(height/2)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  
				 	  roomdata->at(0)[width/2]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  }

	else if (layout == THREE_M_URD) { roomdata->at(0)[width/2]  			= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height/2)[width-1]  		= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == THREE_M_RDL) { roomdata->at(height/2)[width-1]  		= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height/2)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == THREE_M_DLU) { roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height/2)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(0)[width/2]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	else if (layout == THREE_M_LUR) { roomdata->at(height/2)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(0)[width/2]  			= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height/2)[width-2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);  }
	
	else if (layout == FOUR_M_ALL)	{ roomdata->at(0)[width/2]  			= (TileType)(FLOOR | PORTAL_UNLINKED);
					  roomdata->at(height/2)[width-1]  		= (TileType)(FLOOR | PORTAL_UNLINKED);
	 				  roomdata->at(height-1)[width/2]  		= (TileType)(FLOOR | PORTAL_UNLINKED);
	 				  roomdata->at(height/2)[0]  			= (TileType)(FLOOR | PORTAL_UNLINKED);  }
}

void Room::buildCircle(std::vector< std::vector<TileType> > *roomdata)
{
	static const int MIN_RADIUS = 5;
	static const int MAX_RADIUS = 7;

	static const int DOOR_LAYOUTS = 13;
	enum DoorLayouts
	{
		ONE_U	= 0,	TWO_UR	= 4,	THREE_URD = 8,	FOUR_ALL = 12,
		ONE_R	= 1,	TWO_RD	= 5,	THREE_RDL = 9,
		ONE_D	= 2,	TWO_DL	= 6,	THREE_DLU = 10,
		ONE_L	= 3,	TWO_LU	= 7,	THREE_LUR = 11
	};

	int radius  = rand() % MAX_RADIUS + MIN_RADIUS;

	// Build the room
	for (int r = 0; r <= radius * 2; r++)
	{
		std::vector<TileType> temp;
		roomdata->push_back(temp);
		for (int c = 0; c <= radius * 2; c++)
		{
			int cx = (c - radius) * (c - radius);
			int cy = (r - radius) * (r - radius);

			if (cx + cy <= radius * radius)
			{
				if (cx + cy < radius * radius + 1 && cx + cy > radius * radius - 2 * radius)
					roomdata->at(r).push_back(WALL);
				else
					roomdata->at(r).push_back(FLOOR);
			}
			else
			{
				roomdata->at(r).push_back(VOID);
			}
		}	
	}
	
	// Remove the unnecessary walls
	//roomdata->at(0)[radius] = VOID;	// Up
	//roomdata->at(radius)[radius*2] = VOID; // right
	//roomdata->at(radius*2)[radius] = VOID; // down
	//roomdata->at(radius)[0] = VOID; // left

	int layout = rand() % DOOR_LAYOUTS;

	#define makeDoorCustom(r1, c1, r2, c2) \
		roomdata->at(r1)[c1] = (TileType)(FLOOR | PORTAL_UNLINKED);
	#define makeDoorUp() \
		roomdata->at(1)[radius] = (TileType)(FLOOR | PORTAL_UNLINKED);
	#define makeDoorRight()\
		roomdata->at(radius)[radius*2-1] = (TileType)(FLOOR | PORTAL_UNLINKED);
	#define makeDoorDown()\
		roomdata->at(radius*2-1)[radius] = (TileType)(FLOOR | PORTAL_UNLINKED);
	#define makeDoorLeft()\
		roomdata->at(radius)[1] = (TileType)(FLOOR | PORTAL_UNLINKED);

	if	(layout == ONE_U)	{ makeDoorUp(); }
	else if (layout == ONE_R)	{ makeDoorRight(); }
	else if (layout == ONE_D)	{ makeDoorDown(); }
	else if (layout == ONE_L)	{ makeDoorLeft(); }
	else if (layout == TWO_UR)	{ makeDoorUp(); makeDoorRight(); }
	else if (layout == TWO_RD)	{ makeDoorRight(); makeDoorDown(); }
	else if (layout == TWO_DL)	{ makeDoorDown(); makeDoorLeft(); }
	else if (layout == TWO_LU)	{ makeDoorLeft(); makeDoorUp(); }
	else if (layout == THREE_URD)	{ makeDoorUp(); makeDoorRight(); makeDoorDown(); }
	else if (layout == THREE_RDL)	{ makeDoorRight(); makeDoorDown(); makeDoorLeft(); }
	else if (layout == THREE_DLU)	{ makeDoorDown(); makeDoorLeft(); makeDoorUp(); }
	else if (layout == THREE_LUR)	{ makeDoorLeft(); makeDoorUp(); makeDoorRight(); }
	else if (layout == FOUR_ALL)	{ makeDoorUp(); makeDoorRight(); makeDoorDown(); makeDoorLeft(); }
}

void Room::generateHallway(unsigned long x, unsigned long y, std::map<unsigned long, std::map<unsigned long, Tile> > *tiles)
{
	// x and y are location of portal
	srand( (x * y + time(NULL) % 5) >> 1);

	// Generation Direction
	enum GenDir
	{
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 3
	};

	// To begin, we want to take the hallway at least one tile AWAY from the room so it doesn't replace any wall tile
	// Check to see where the last floor tile is.  This will tell us which direction our door is facing relative to the room it was generated in
	// We'll have to make sure that we aren't accessing undirialized data
	// TODO: Make hallways bordered by walls

	#define tileUpExists(x,y,i)\
		tiles->find(x)->second.find(y-i) != tiles->find(x)->second.end()
	#define tileRightExists(x,y,i)\
		tiles->find(x+i) != tiles->end()
	#define tileDownExists(x,y,i)\
		tiles->find(x)->second.find(y+i) != tiles->find(x)->second.end()
	#define tileLeftExists(x,y,i)\
		tiles->find(x-i) != tiles->end()

	GenDir dir;
	Tile t(FLOOR);
	// Check for floor below.  If so, we want to set the tile above the portal to a floor
	if 	(tileDownExists(x,y,1) && tiles->at(x).at(y+1).type == FLOOR)
	{	
		// Check to see if a tile already exists in the path
		if (!( tileUpExists(x,y,1) ))
		{
			// if not, insert tile
			(*tiles)[x].insert( std::pair<unsigned long, Tile>(y-1, t) );
		}
		else
		{
			// If so, replace the tile's properties
			tiles->at(x).at(y-1).changeType(FLOOR);
		}
		dir = UP;
	}
	// check for floor to left.  If so, set right tile to floor
	else if (tileLeftExists(x,y,1) && tiles->at(x-1).at(y).type == FLOOR)
	{
		if (!( tileRightExists(x,y,1) ))
		{
			(*tiles)[x+1].insert( std::pair<unsigned long, Tile>(y, t) );
		}
		else
		{
			(*tiles).at(x+1).at(y).changeType(FLOOR);
		}
		dir = RIGHT;
	}
	// check for floor above.  If so, set below tile to floor
	else if (tileUpExists(x,y,1) && tiles->at(x).at(y-1).type == FLOOR)
	{
		if (!( tileDownExists(x,y,1) ))
		{
			tiles->at(x).insert( std::pair<unsigned long, Tile>(y+1, t) );
		}
		else
		{
			tiles->at(x).at(y+1).changeType(FLOOR);
		}
		dir = DOWN;
	}
	// check for floor to right.  If so, set left tile to floor
	else if (tileRightExists(x,y,1) && tiles->at(x+1).at(y).type == FLOOR)
	{
		if (!( tileLeftExists(x,y,1) ))
		{
			(*tiles)[x-1].insert( std::pair<unsigned long, Tile>(y, t) );
		}
		else
		{
			tiles->at(x-1).at(y).changeType(FLOOR);
		}
		dir = LEFT;
	}

	bool done = false;
	while (!done)
	{
		// Pick a random direction to generate in, along with a random number of tiles to generate
		// Make sure that we don't generate backward
		GenDir prevDir = dir;
		GenDir dir = (GenDir)(rand() % 4);
		
		bool validDirection = false;
		while ( !validDirection )
		{
			if	( (dir == UP && prevDir == DOWN) || (dir == DOWN && prevDir == UP) ||
				  (dir == RIGHT && prevDir == LEFT) || (dir == LEFT && prevDir == RIGHT) )
			{
				dir = (GenDir)( rand() % 4 );
			}
			else validDirection = true;
		}

		#define MAX_LENGTH 10
		#define MIN_LENGTH 4
		int segmentLength = rand() % MAX_LENGTH + MIN_LENGTH;
		unsigned long prevx, prevy;
		for (int i = 0; i < segmentLength; i++)
		{
			Tile newTile(FLOOR);
			if 	(dir == UP)
			{
				if (!( tileUpExists(x,y,i) ))	tiles->at(x).insert( std::pair<unsigned long, Tile>(y-i, newTile) );
				else				tiles->at(x).at(y-i).changeType(FLOOR);
			}
			else if (dir == RIGHT)
			{
				if (!( tileRightExists(x,y,i) ))	tiles->at(x+i).insert( std::pair<unsigned long, Tile>(y, newTile) );
				else					tiles->at(x+i).at(y).changeType(FLOOR);
			}
			else if (dir == DOWN)
			{
				if (!( tileDownExists(x,y,i) ))	tiles->at(x).insert( std::pair<unsigned long, Tile>(y+i, newTile) );
				else				tiles->at(x).at(y+i).changeType(FLOOR);
			}
			else if (dir == LEFT)
			{
				if (!( tileLeftExists(x,y,i) ))	tiles->at(x-i).insert( std::pair<unsigned long, Tile>(y, newTile) );
				else				tiles->at(x-i).at(y).changeType(FLOOR);
			}
		}

		// 30% chance to be done generating hallway
		done = ( (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < 0.3f );
	}
}
