#ifndef __SAVEMAP_H__
#define __SAVEMAP_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "Map.h"

#define BUFF_SIZE 30000

void probeEntity(const char* raw_line, std::vector<Entity*>* entities, Map* load_location_map, ResourcesManager* resources_manager);

void probeTileset(const char* raw_line, Tile* tileset, int tileset_size);

void probeMap(const char* raw_line, Map* load_location_map);

void probeHeader(const char* raw_line, sf::Vector2f* size_map, int* height_map, int* tile_sz_map, char texture_name_file[BUFF_SIZE]);

int loadMap(Map** load_location_map, std::vector<Entity*>* entities_to_load, std::string name_file_map_to_load, std::string* name_texture, sf::Texture** texture, Tile** tileset, int* tileset_size, ResourcesManager* resources_manager);

int saveMap(const Map* map_to_save, const std::vector<Entity*> entities_to_save, const std::string name_map, const std::string name_file, const std::string texture_name_file, const Tile* tileset, const int tileset_size);

int loadEntity(std::string entity_id, std::string file_name, ResourcesManager* resources_manager, Entity** entity_to_load, Map* entity_location);



#endif