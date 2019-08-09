#ifndef __TOOLS_MAPCREATOR_H__
#define __TOOLS_MAPCREATOR_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <dirent.h>

#include "Map.h"
#include "saveMap.h"


void list_entities(std::vector<std::string>* entities_files);
void saveMapCommand(Map* map_to_save, std::vector<Entity*> entities_to_save, std::string texture_name, Tile* tileset, int tileset_size);
int loadMapCommand(sf::RenderWindow* main_window, Map** map_to_load, std::vector<Entity*>* entities_to_load, sf::Texture* texture, Tile** tileset, int* tileset_size, bool fullscreen);
void introText(void);




#endif