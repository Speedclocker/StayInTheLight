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
	int* nbr_avail_tiles; 
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(sf::Texture*) - sizeof(Tile*) - sizeof(int*) ] = { 0 };
} ArgTilesetWindow;



//////////// Pour l'onglet de           //////////////

// Fonction



// Argument


#endif