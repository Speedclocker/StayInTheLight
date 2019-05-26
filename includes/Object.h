#ifndef __OBJECT_H__
#define __OBJECT_H__


#include <SFML/Graphics.hpp>
#include <iostream>

class Object : public sf::Drawable
{
public:
	Object();
	~Object();	

	std::string getType();

	virtual sf::Vector2f getPosition();
	virtual sf::Vector2f getSize();

protected:
	std::string m_type;
	sf::Vector2f m_position;
	int m_height;
	sf::Vector2f m_size;
};

#endif
