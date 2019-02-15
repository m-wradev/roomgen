#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

// Headers with no implementation
#include "symbols.h"
#include "tiles.h"

// Classes and headers with implementation
//#include "room.h"
#include "world.h"

/*		TODO
 * Generate hallways to other rooms
 *
 * Look mode and move/attack mode
 *
 * Menus and menu selection
 *
 * Currency (gold, silver, etc)
 *
 * Weapons and items
 *
 * Find a way to register keypresses (maybe)
 */


using namespace std;

// SCREEN SETTINGS
struct winsize size;

static int H_CAM_DIST;
static int V_CAM_DIST = 32;

struct Player
{
	static const int VIEW_RADIUS = 6;
	static const int MAX_HEALTH  = 100;

	string name;
	unsigned long x, y;
	int health;
	int lvl;
	int mvspeed = 1;

	Player()
	{

		cout << "Enter a name for your character: ";
		cin >> name;

		health = MAX_HEALTH;
		lvl = 1;

		srand(time(NULL));

		x = rand() % 10000 + 5000;
		y = rand() % 10000 + 5000;
		//x = 0;
		//y = 0;
		health = 100;
	}
	
	// Set tiles within the player's view distance as discovered
	void look(map<unsigned long, map<unsigned long, Tile> > *tiles)
	{
		for (unsigned long c = x - VIEW_RADIUS; c < x + VIEW_RADIUS; c++)
		{
			for (unsigned long r = y - VIEW_RADIUS; r < y + VIEW_RADIUS; r++)
			{
				if (r < 0 || c < 0) continue;

				if (tiles->find(c) == tiles->end()) continue;
				if (tiles->at(c).find(r) == tiles->at(c).end()) continue;

				signed int cx = (c - x) * (c - x); // (x - h)^2
				signed int cy = (r - y) * (r - y); // (y - k)^2
				if (cx + cy <= VIEW_RADIUS * VIEW_RADIUS)
				{
					tiles->at(c).at(r).discovered = true;
					
				}
			}
		}
	}
};

struct Level
{
	//	  x-pos          y-pos 		tile
	map< unsigned long, map<unsigned long, Tile> > tiles;
	
	/*
	Level(Player *p)
	{
		// The player will always start out in a room
		vector< vector<TileType> > starting_room;
		starting_room = Room::buildRoom();

		// The top left corner of the room is always placed at the player's coordinates
		for (unsigned long r = 0; r < starting_room.size(); r++) 
		{
			for (unsigned long c = 0; c < starting_room.at(r).size(); c++)
			{
				Tile t( starting_room.at(r).at(c) );
				
				// Insert x, then y and tile
				tiles[p->x + c].insert( pair<unsigned long, Tile>(p->y + r, t) );
			}
		}

		// Move the player to the middle of the new room
		p->x = p->x + starting_room.at(0).size() / 2;
		p->y = p->y + starting_room.size() / 2;
	}
	*/

	Level(Player *p)
	{
		vector< vector<TileType> > world = World::createWorld();

		for (unsigned long r = 0; r < world.size(); r++)
		{
			for (unsigned long c = 0; c < world.at(r).size(); c++)
			{
				Tile t( world.at(r).at(c) );
				tiles[p->x + c].insert( pair<unsigned long, Tile>(p->y + r, t) );
			}
		}

		// Place the player at the spawn point
		p->x = p->x + World::pSpawn_x;
		p->y = p->y + World::pSpawn_y;
	}

	vector< vector<string> > getScene(Player *p)
	{
		vector< vector<string> > scene;
		for (int r = p->y - V_CAM_DIST/2; r < p->y + V_CAM_DIST/2; r++)
		{	
			vector<string> line;
			scene.push_back(line);
			for (int c = p->x - H_CAM_DIST/2; c < p->x + H_CAM_DIST/2; c++)
			{
				if (r < 0) continue;
				if (c < 0) continue;

				auto i = tiles.find(c);
				if (i == tiles.end())
				{
					scene.back().push_back(Symbols::TILE_VOID);
					continue;
				}

				auto j = i->second.find(r);
				if (j == i->second.end())
				{
					scene.back().push_back(Symbols::TILE_VOID);
					continue;
				}


				// If the current symbol is within the player's view distance, draw it in the regular color
				// Else, make the symbol dark gray (this represents areas the player has explored but cannot see)
				// TODO: Make it so that the player cannot see through walls
				string symbol;	

				// Make any symbols that are outside of a circle made by the player's view radius gray	
				int cx  = (c - p->x) * (c - p->x);
				int cy  = (r - p->y) * (r - p->y);
				if (cx + cy > p->VIEW_RADIUS * p->VIEW_RADIUS)
				{
					if (j->second.discovered)
						symbol = "\033[1;30m" + j->second.symbol + "\033[0m";
					else
						symbol = Symbols::TILE_VOID;
				}
				else
				{	
					symbol = (c == p->x && r == p->y) ? SymbolColors::ENT_PLAYER + Symbols::ENT_PLAYER : j->second.color + j->second.symbol;
				}
				scene.back().push_back(symbol);
			}		
		}
		return scene;
	}

	// Check if a tile is a wall
	// Return true if a tile is a wall, false otherwise
	bool checkCollision(unsigned long x, unsigned long y)
	{
		bool exists = (tiles.find(x) != tiles.end());
		if ( tiles.find(x) != tiles.end() && tiles.at(x).find(y) != tiles.at(x).end() )
		{
			Tile t = tiles.at(x).at(y);
			return (t.type == WALL || t.type == VOID);
		}
		return true;
	}
};

// Function prototypes
void update();
void render(Player *p, Level *level);

int main()
{
	// Clear the screen and set cursor to top
	cout << "\033[2J\033[1;1H";

	srand(time(NULL));
	
	// Create the player at a random location
	Player player;
	Level level(&player);	

	// Get size of terminal
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	H_CAM_DIST = size.ws_col - 2;

	render(&player, &level);
	string input;
	getline(cin, input);
	while ( input != "q" ) // Main game loop
	{
		// Get input from the player

		// move up
		if 	( input == "w" && !level.checkCollision(player.x, player.y - player.mvspeed) ) { player.y = player.y - player.mvspeed; }
		// move down
		else if ( input == "s" && !level.checkCollision(player.x, player.y + player.mvspeed) ) { player.y = player.y + player.mvspeed; }
		// move left
		else if ( input == "d" && !level.checkCollision(player.x + player.mvspeed, player.y) ) { player.x = player.x + player.mvspeed; }
		// move right
		else if ( input == "a" && !level.checkCollision(player.x - player.mvspeed, player.y) ) { player.x = player.x - player.mvspeed; }
		
		update();
		render(&player, &level);

		cout << "Enter command: ";
		getline(cin, input);
	}

	cout << "\033[2J\033[1;1H\033[0m";
	cout << "Thank you for playing." << endl;
	cout << "Created by Ryan Armstrong" << endl;
	cout << endl;

	return 0;
}

void update()
{

}

void render(Player *p, Level *level)
{
	// Always check to make sure the window hasn't been resized
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	H_CAM_DIST = size.ws_col - 2;

	// Let player discover tiles so they can be drawn
	p->look(&level->tiles);

	cout << "\033[2J\033[1;1H"; // Clear the screen and put the cursor at position 1, 1
	vector< vector<string> > scene = level->getScene(p);

	int screen_width  = scene.size() + 2;
	int screen_height = scene.at(0).size() + 2;

	for (int x = 0; x < screen_width; x++)
	{
		for (int y = 0; y < screen_height; y++)
		{
			if ( (x == 0 || y == 0) || (x == screen_width - 1) || (y == screen_height - 1) )
			{
				// Print screen border
				cout << "\033[0;42m \033[0m";
			}
			else if (x < screen_width - 1 && y < screen_height - 1)
			{
				// Print the border
				cout << scene[x-1][y-1];
			}

		}
		cout << endl;
	}

	// print the status bar
	// name, level, life
	printf("%s\t\t\033[1;32mLevel:\033[0m %d\t\t\033[1;32mLife:\033[0m %d/100\t\t(x: %d, y: %d)\n", p->name.c_str(), p->lvl, p->health, p->x, p->y);

}
