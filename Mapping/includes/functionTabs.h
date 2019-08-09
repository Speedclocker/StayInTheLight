#ifndef __FUNCTIONTABS_H__
#define __FUNCTIONTABS_H__


#include <SFML/Graphics.hpp>
#include "BoxWindow.h"



////////////// Tileset Tab ////////////

class ResourcesManager;

// Function
void TilesetTab(Tab* tab, ArgTab* argtab);

// Argument Structure
typedef struct
{
	sf::Texture* texture; 
	Tile** tiles; 
	const int* nbr_avail_tiles;
	const int* size_tile;
	Tile* target_tile;
	std::string** text_event_location;
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(sf::Texture*) - sizeof(Tile**) - sizeof(const int*) - sizeof(const int*) - sizeof(Tile*) - sizeof(std::string**)] = { 0 };
} ArgTilesetTab;





/////////////// Info Tab ///////////////

// Function
void InfoTab(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	Map**					ptr_map;
	std::vector<Entity*>*	entities;
	uint8_t 				__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(Map**) - sizeof(std::vector<Entity*>*)] = {0};
} ArgInfoTab;






/////////////// Entities Tab ///////////////

// Function
void EntitiesTab(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	std::vector<std::string>* 	ptr_list_entities;
	Entity**					current_entity;
	std::string* 				current_entity_file_name;
	Map*						map;
	ResourcesManager* 			resources_manager;
	uint8_t 					__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(std::vector<std::string>*) - sizeof(Entity**) - sizeof(std::string*) - sizeof(Map*) - sizeof(ResourcesManager*)] = {0};
} ArgEntitiesTab;



#endif