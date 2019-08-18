#ifndef __ENTITY_H__
#define __ENTITY_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstring>

#define SITL_FPS_QUOTIENT 4

class AnimatedSprite;
class ResourcesManager;


enum Sense { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, DEFAULT_SENSE };


class Entity : public sf::Drawable
{
public:

	// Constructors and Destructors
	Entity();
	virtual ~Entity() {};	


	// Getters
	std::string 					getID();
	std::string 					getModelName();
	std::string 					getType() const;
	sf::Texture* 					getTexture();
	sf::Vector2f 					getPosition() const;
	sf::Vector2f 					getSize() const;
	int 							getHeight() const;
	int 							getGroundZone() const;
	Sense 							getSense() const;
	AnimatedSprite*		 			getSprite();	
	bool 							isAffiliatedToMap();


	// Setters
	void 							setModelName(std::string name);
	void 							setType(std::string type);
	virtual void 					setTexture(sf::Texture* texture);
	void 							setPosition(sf::Vector2f position);
	void 							setHeight(int height);
	void 							setSize(sf::Vector2f size);
	void 							setGroundZone(int ground_zone);
	void 							setSense(Sense sense);


	// Methods
	int 							loadFromFile(std::string file_name, sf::Texture* texture);
	int 							loadFromFile(std::string file_name, ResourcesManager* resources_manager);
	virtual void 					update();
	virtual void 					drawPart(sf::RenderWindow* window, unsigned int height);
	virtual void 					drawPartAndAbove(sf::RenderWindow* window, unsigned int height);


protected:
	virtual void 					readFeaturesFromString(std::string string);
	virtual void 					readAnimationFromString(std::string string);


	std::string 			m_id; 				// The id of the entity
	std::string 			m_model_name; 		// The model of entity (is it a palm? a car?)
	AnimatedSprite* 		m_sprite;			// Sprite of a character entity
	bool 					m_affiliated_to_map;// If the sprite is affiliated to a map, return true;
	sf::Texture* 			m_texture;			// Texture associated with the entity
	std::string 			m_type;				// Type of the entity (is it a Character, a Collector...)
	sf::Vector2f 			m_position;			// Absolute position of the entity
	sf::Vector2f 			m_size;				// Size of an entity
	int 					m_height;			// Level in height of the entity
	int 					m_ground_zone;		// Length of an entity in contact with ground (if the entity was a cube, ground_zone corresponds to the y length of the face parallel to the ground )
	Sense 					m_sense; 			// Sense of an entity
};

#endif
