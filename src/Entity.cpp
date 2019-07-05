#include "Entity.h"


Entity::Entity()
{

}

Entity::~Entity()
{

}



// Getters

std::string Entity::getType() const
{
	/* Return the type of the entity */
	return m_type;
}


sf::Vector2f Entity::getPosition() const
{
	/* Return the position of the entity */
	return m_position;
}


sf::Vector2f Entity::getSize() const
{
	/* Return the size of the entity */
	return m_size;
}


int Entity::getHeight() const
{
	/* Return the height of the entity */
	return m_height;
}


int Entity::getGroundZone() const
{
	/* Return the length of the zone parallel with the ground */
	return m_ground_zone;
}



// Setters

void Entity::setType(std::string type)
{
	/* Change the type of the entity */
	m_type = type;
}


void Entity::setPosition(sf::Vector2f position)
{
	/* Change the position of the entity */
	m_position = position;
}


void Entity::setHeight(int height)
{
	/* Change the height of the entity */
	m_height=height;
}


void Entity::setSize(sf::Vector2f size)
{	
	/* Change the size of the entity */
	m_size = size;
}


void Entity::setGroundZone(int ground_zone)
{
	/* Change the length of the zone parallel with the ground */
	m_ground_zone = ground_zone;
}



// Methods

void Entity::drawPart(sf::RenderWindow* window, unsigned int height)
{
	/* For sub-class, draw a part of sprite corresponding to the height given in parameters */
	(void)height;
	(void)window;
}


void Entity::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	/* For sub-class, draw all the sprite above a height */
	(void)height;
	(void)window;
}