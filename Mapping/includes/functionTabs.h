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





/////////////// Info Tab ///////////////

// Function
void InfoWindow(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	Map*				map;
	uint8_t 			__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(Map*)] = {0};
} ArgInfoWindow;






/////////////// Entities Tab ///////////////

// Function
void EntitiesWindow(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	std::vector<std::string>* 	ptr_list_entities;
	Entity**					current_entity;
	Map*						map;
	uint8_t 					__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(std::vector<std::string>*) - sizeof(Entity**) - sizeof(Map*)] = {0};
} ArgEntitiesWindow;



#endif