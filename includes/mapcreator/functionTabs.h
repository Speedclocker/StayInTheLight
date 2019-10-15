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
	sf::Texture** texture; 
	Tile** tiles; 
	const int* nbr_avail_tiles;
	const int* size_tile;
	Tile* target_tile;
	ResourcesManager* ptr_resources_manager;
	std::string** text_event_location;
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(sf::Texture**) - sizeof(Tile**) - sizeof(const int*) - sizeof(const int*) - sizeof(Tile*) - sizeof(ResourcesManager*) - sizeof(std::string**)] = { 0 };
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
	bool* 						set_or_erase_option;
	std::string** 				text_event_location;
	uint8_t 					__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(std::vector<std::string>*) - sizeof(Entity**) - sizeof(std::string*) - sizeof(Map*) - sizeof(ResourcesManager*) - sizeof(bool*) - sizeof(std::string**)] = {0};
} ArgEntitiesTab;




/////////////// Save Tab ///////////////

// Function
void SaveTab(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	Map** map_to_save; 
	const std::vector<Entity*>* ptr_entities_to_save;
	const std::string* ptr_texture_name_file;
	Tile** ptr_tileset_to_save;
	const int* nbr_tiles;
	int* save_state;
	std::string** text_event_location;
	uint8_t 					__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(Map**) - sizeof(const std::vector<Entity*>*) - sizeof(const std::string*) - sizeof(Tile**) - sizeof(const int*) - sizeof(int*) - sizeof(std::string**)] = {0};
} ArgSaveTab;




/////////////// Load Tab ///////////////

// Function
void LoadTab(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	Map** map_to_load; 
	std::vector<Entity*>* ptr_entities_to_load;
	std::string* ptr_texture_name_file;
	sf::Texture** texture_to_load;
	Tile** ptr_tileset_to_load;
	int* nbr_tiles;
	ResourcesManager** ptr_resources_manager;
	int* load_state;
	std::string** text_event_location;
	uint8_t 					__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(Map**) - sizeof(std::vector<Entity*>*) - sizeof(std::string*) - sizeof(sf::Texture**) - sizeof(Tile**) - sizeof(int*) - sizeof(ResourcesManager**) - sizeof(int*) - sizeof(std::string**)] = {0};
} ArgLoadTab;




////////////// New Tab ////////////////


// Function
void NewMapTab(Tab* tab, ArgTab* argtab);

// Argument Structure

typedef struct
{
	Map** new_map; 
	std::vector<Entity*>* ptr_new_entities;
	std::string* ptr_texture_name_file;
	sf::Texture** new_texture;
	ResourcesManager* ptr_resources_manager;
	int* new_map_state;
	std::string** text_event_location;
	uint8_t 					__argtab_offset[ARG_TAB_BUFF_SIZE - sizeof(Map**) - sizeof(std::vector<Entity*>*) - sizeof(std::string*) - sizeof(sf::Texture**) - sizeof(ResourcesManager*) - sizeof(int*) - sizeof(std::string**)] = {0};
} ArgNewMapTab;





/////////// EntityCreator Tab ///////////////


// Function
void EntityCreatorTab(Tab* tab, ArgTab* argtab)

// Argument Structure

typedef struct
{
	uint8_t __argtab_offset[ARG_TAB_BUFF_SIZE] = {0};
} ArgEntityCreatorTab;

#endif
