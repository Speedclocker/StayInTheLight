#include "Object.h"


Object::Object()
{

}

Object::~Object()
{

}


std::string Object::getType()
{
	return m_type;
}



sf::Vector2f Object::getPosition()
{
	//Retourne la position de l'objet
	return m_position;
}


sf::Vector2f Object::getSize()
{
	//Retourne la taille de l'objet
	return m_size;
}