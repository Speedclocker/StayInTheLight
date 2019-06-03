#ifndef __TOOLS_MAPCREATOR_H__
#define __TOOLS_MAPCREATOR_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"
#include "saveMap.h"
#define FONT_FILE "AldoTheApache.ttf"

/* Ce fichier l'ensemble des outils utilisés par MapCreator, que ce soit la mise en place des tiles, le déplacement entre les différentes couches etc...*/


void choice_tile(Tile* tile_target, sf::RenderWindow* window, sf::Texture* texture_file, Tile tiles[], int nbr_avail_tiles, int size_tile);
void tileset_collision_settings(sf::RenderWindow* window, sf::Texture* texture_file, Tile tiles[], int nbr_avail_tiles, int size_tile);
void control_view(sf::RenderWindow* window, sf::View* view, Map* map);
void set_tile(sf::RenderWindow* window, sf::Texture *texture, Map* map, Tile tile, int height);
bool height_settings(sf::RenderWindow* window, Map* map, int* chosen_height);
void save_map(Map* map_to_save, std::string texture_name);
int load_map(sf::RenderWindow* main_window, Map** map_to_load, sf::Texture* texture, Tile** available_tiles, int* nbr_tiles, bool fullscreen);
void introText(void);




#endif