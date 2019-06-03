#ifndef __FUNCTIONTABS_H__
#define __FUNCTIONTABS_H__


#include <SFML/Graphics.hpp>
#include "BoxWindow.h"



////////////// Pour l'onglet des tileset ////////////

// Fonction
void TilesetWindow(Tab* tab, ArgTab* argtab);

// Argument
typedef struct
{
	sf::Texture* texture; 
	Tile* tiles; 
	const int* nbr_avail_tiles;
	const int* size_tile;
	Tile* target_tile;
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(sf::Texture*) - sizeof(Tile*) - sizeof(int*) - sizeof(int*) - sizeof(Tile*)] = { 0 };
} ArgTilesetWindow;



//////////// Pour l'onglet de           //////////////

// Fonction



// Argument


#endif