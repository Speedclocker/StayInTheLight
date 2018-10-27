#include <SFML/Graphics.hpp>
#include <iostream>
#include "Interactions.h"



void character_key_input(Character* character, bool* attack)
{
	// Déplacement du personnage
	
	int speed = character->getSpeed();

	if(character->getState()!=ATTACKING)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
		character->move(sf::Vector2f(+sqrt(speed), -sqrt(speed)));
		character->setSense(UP_RIGHT);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
		character->move(sf::Vector2f(-sqrt(speed), -sqrt(speed)));
		character->setSense(UP_LEFT);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
		character->move(sf::Vector2f(+sqrt(speed), +sqrt(speed)));
		character->setSense(DOWN_RIGHT);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
		character->move(sf::Vector2f(-sqrt(speed), +sqrt(speed)));
		character->setSense(DOWN_LEFT);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
		character->move(sf::Vector2f(0, -speed));
		character->setSense(UP);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
		character->move(sf::Vector2f(0, speed));
		character->setSense(DOWN);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
		character->move(sf::Vector2f(-speed, 0));
		character->setSense(LEFT);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
		character->move(sf::Vector2f(speed, 0));
		character->setSense(RIGHT);
		}
	}

	if(*attack)
	{
		character->attack();
		*attack=0;
	}
            
            
}
