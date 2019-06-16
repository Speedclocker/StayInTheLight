#ifndef __ENTITY_H__
#define __ENTITY_H__


#include <SFML/Graphics.hpp>
#include <iostream>

class Entity : public sf::Drawable
{
public:
	Entity();
	~Entity();	

	std::string getType();

	virtual sf::Vector2f getPosition();
	virtual sf::Vector2f getSize();
	virtual int getHeight();
	int getGroundZone();

	virtual void drawPart(sf::RenderWindow* window, unsigned int height);
	virtual void drawPartAndAbove(sf::RenderWindow* window, unsigned int height);


protected:
	std::string m_type;
	sf::Vector2f m_position;
	int m_height;
	int m_ground_zone;
	sf::Vector2f m_size;
};

#endif
