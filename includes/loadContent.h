#ifndef __SAVEMAP_H__
#define __SAVEMAP_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#include "Map.h"
#include "Entity.h"

#define BUFF_SIZE 30000


void probeMap(const char* raw_line, Map* load_location_map);

void probeHeader(const char* raw_line, sf::Vector2f* size_map, int* height_map, int* tile_sz_map, char texture_file_name[BUFF_SIZE]);

int loadMap(Map** load_location_map, std::string map_to_load_file_name, std::string* name_texture);



void probeEntityHeader(const char* raw_line, sf::Vector2f* size_map, int* height_map, int* tile_sz_map, char texture_file_name[BUFF_SIZE]);

int loadEntity(Entity* entity, std::string entity_to_load_file_name);


#endif