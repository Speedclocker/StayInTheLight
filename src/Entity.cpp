#include "Entity.h"


Entity::Entity()
{

}

Entity::~Entity()
{

}


std::string Entity::getType()
{
	return m_type;
}



sf::Vector2f Entity::getPosition()
{
	//Retourne la position de l'objet
	return m_position;
}


sf::Vector2f Entity::getSize()
{
	//Retourne la taille de l'objet
	return m_size;
}

int Entity::getHeight()
{
	return m_height;
}

int Entity::getGroundZone()
{
	return m_ground_zone;
}


void Entity::drawPart(sf::RenderWindow* window, unsigned int height)
{
	(void)height;
	(void)window;
}


void Entity::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	(void)height;
	(void)window;
}