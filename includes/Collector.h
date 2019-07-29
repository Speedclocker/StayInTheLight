#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "collision.h"
#include "Animation.h"
#include "Entity.h"


#define PI 3.14159



class Collector : public Entity
{
public:
	enum State { MOVING, STANDING, DEFAULT_STATE };


	// Constructors and destructors
	Collector();
	Collector(sf::Texture* texture, Map* map);
	Collector(std::string file_name, sf::Texture* texture, Map* map);

	virtual ~Collector();


	// Getters
	int 					getHealth();
	int 					getSpeed();
	sf::IntRect 			getHitbox();
	sf::IntRect 			getAbsHitbox();
	State 					getState() const;
	sf::Clock 				getClock();
	//bool 					isAffiliatedToMap();


	// Setters 
	void 					setTexture(sf::Texture* texture);
	void 					setHealth(int health);
	void 					setSpeed(int speed);
	void 					setHitbox(sf::IntRect hitbox);
	void 					setState(State state);


	// Methodes
	void 					update();
	void 					move(sf::Vector2f movement);
	void 					move(int mov_x, int mov_y);
	void 					takeDamages(int damages);
	void 					drawPart(sf::RenderWindow* window, unsigned int height);
	void 					drawPartAndAbove(sf::RenderWindow* window, unsigned int height);


protected:
	virtual void 			readFeaturesFromString(std::string string);
	virtual void 			readAnimationFromString(std::string string);


private:
	virtual void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


	// Attributes
	int 																		m_collectible; 				// What kind of collectible can leave the collector
	int 																		m_health;					// Health of an entity collector
	int 																		m_speed;					// Speed of an entity collector
	//bool 																		m_affiliated_to_map; 		// If the sprite is affiliated to a map, return true;
	std::map< std::pair<Collector::State, Sense> , AnimationParameters > 		m_animation_parameters;		// Animation parameters of an enitity collectors identified with the state and the sense of entity
	sf::IntRect 																m_hitbox;					// Hitbox of the entity
	Collector::State 															m_state;					// State of the entity
	sf::Clock 																	m_clock;					// Clock associated to the entity, to evaluate its time;
};




#endif