#ifndef __FUNCTIONTABS_H__
#define __FUNCTIONTABS_H__


#include <SFML/Graphics.hpp>
#include "BoxWindow.h"



////////////// Tileset Tab ////////////

// Function
void TilesetWindow(Tab* tab, ArgTab* argtab);

// Argument Structure
typedef struct
{
	sf::Texture* texture; 
	Tile** tiles; 
	int* nbr_avail_tiles;
	int* size_tile;
	Tile* target_tile;
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(sf::Texture*) - sizeof(Tile**) - sizeof(int*) - sizeof(int*) - sizeof(Tile*)] = { 0 };
} ArgTilesetWindow;



//////////// 				//////////////

// Function
//void InfoWindow(Tab* tab, ArgTab* argtab);

// Argument Structure
/*
typedef struct
{
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE] = {0};
} ArgInfoWindow;
*/
#endif