#include "Entity.h"


Entity::Entity()
{

}

Entity::~Entity()
{

}

//Getters

std::string Entity::getType()
{
	// Return the type of the entity
	return m_type;
}



sf::Vector2f Entity::getPosition()
{
	// Return the position of the entity
	return m_position;
}


sf::Vector2f Entity::getSize()
{
	// Return the size of the entity
	return m_size;
}

int Entity::getHeight()
{
	// Return the height of the entity
	return m_height;
}

int Entity::getGroundZone()
{
	// Return the integer corresponding to the zone in contact of the ground (useful for Animated sprite in map allowinng perspective)
	return m_ground_zone;
}



// Methods

void Entity::drawPart(sf::RenderWindow* window, unsigned int height)
{
	// For sub-class, draw a part of sprite corresponding to the height given in parameters
	(void)height;
	(void)window;
}


void Entity::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	// For sub-class, draw all the sprite above a height
	(void)height;
	(void)window;
}