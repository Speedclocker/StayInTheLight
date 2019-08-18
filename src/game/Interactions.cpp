#include <SFML/Graphics.hpp>
#include <iostream>
#include "Interactions.h"



void character_key_input(Character* character, bool* attack)
{
	// Get keys to move the character
	
	int speed = character->getSpeed();
	float diag_speed = sqrt(2)/2*speed;

	if(character->getState()!=Character::ATTACKING)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			character->move(sf::Vector2f(+diag_speed, -diag_speed));
			character->setSense(UP_RIGHT);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			character->move(sf::Vector2f(-diag_speed, -diag_speed));
			character->setSense(UP_LEFT);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			character->move(sf::Vector2f(+diag_speed, +diag_speed));
			character->setSense(DOWN_RIGHT);
			character->setState(Character::MOVING);	
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			character->move(sf::Vector2f(-diag_speed, +diag_speed));
			character->setSense(DOWN_LEFT);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			character->move(sf::Vector2f(0, -speed));
			character->setSense(UP);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			character->move(sf::Vector2f(0, speed));
			character->setSense(DOWN);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			character->move(sf::Vector2f(-speed, 0));
			character->setSense(LEFT);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			character->move(sf::Vector2f(speed, 0));
			character->setSense(RIGHT);
			character->setState(Character::MOVING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::U))
		{
			character->setHeight(0);
			character->setState(Character::STANDING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I))
		{
			character->setHeight(1);
			character->setState(Character::STANDING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		{
			character->setHeight(2);
			character->setState(Character::STANDING);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			character->setHeight(3);
			character->setState(Character::STANDING);
		}
		else
			character->setState(Character::STANDING);
	}

	if(*attack)
	{
		character->attack();
		*attack=0;
	}
            
            
}
