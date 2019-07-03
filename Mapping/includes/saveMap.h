#ifndef __SAVEMAP_H__
#define __SAVEMAP_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#include "Map.h"

#define BUFF_SIZE 30000

void probeTileset(const char* raw_line, Tile* tileset, int tileset_size);

void probeMap(const char* raw_line, Map* load_location_map);

void probeHeader(const char* raw_line, sf::Vector2f* size_map, int* height_map, int* tile_sz_map, char texture_name_file[BUFF_SIZE]);

int loadMap(Map** load_location_map, std::string name_file_map_to_load, std::string* name_texture, sf::Texture* texture, Tile** tileset, int* tileset_size);

int saveMap(Map* map_to_save, std::string name_map, std::string name_file, std::string texture_name_file, Tile* tileset, int tileset_size);




#endif