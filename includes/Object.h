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

protected:
	std::string m_type;
};

#endif
