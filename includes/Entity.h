#ifndef __ENTITY_H__
#define __ENTITY_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstring>



enum Sense { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, DEFAULT_SENSE };


class Entity : public sf::Drawable
{
public:

	// Constructors and Destructors
	Entity();
	~Entity();	


	// Getters
	sf::Texture* getTexture();
	std::string getType() const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	int getHeight() const;
	int getGroundZone() const;
	Sense getSense() const;


	// Setters
	virtual void setTexture(sf::Texture* texture);
	void setType(std::string type);
	void setPosition(sf::Vector2f position);
	void setHeight(int height);
	void setSize(sf::Vector2f size);
	void setGroundZone(int ground_zone);
	void setSense(Sense sense);


	// Methods
	int loadFromFile(std::string file_name, sf::Texture* texture);
	virtual void drawPart(sf::RenderWindow* window, unsigned int height);
	virtual void drawPartAndAbove(sf::RenderWindow* window, unsigned int height);


protected:
	virtual void readFeaturesFromString(std::string string);
	virtual void readAnimationFromString(std::string string);

	sf::Texture* 			m_texture;			// Texture associated with the entity
	std::string 			m_type;				// Type of the entity (is it a Character, a Collector...)
	sf::Vector2f 			m_position;			// Absolute position of the entity
	sf::Vector2f 			m_size;				// Size of an entity
	int 					m_height;			// Level in height of the entity
	int 					m_ground_zone;		// Length of an entity in contact with ground (if the entity was a cube, ground_zone corresponds to the y length of the face parallel to the ground )
	Sense 					m_sense; 			// Sense of an entity
};

#endif
