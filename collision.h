#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <SFML/Graphics.hpp>

class Character;


bool collision_rects(sf::IntRect rect_1, sf::IntRect rect_2);
bool collision_characters(Character character_1, Character character_2);
void physics_characters(Character* character_1, Character* character_2);

#endif