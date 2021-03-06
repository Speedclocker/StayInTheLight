#ifndef __CHARACTER_H__
#define __CHARACTER_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "collision.h"
#include "Animation.h"
#include "Entity.h"


#define PI 3.14159




class Character;

// Class Attack 

class Attack
{
public:
	
	//Constructeurs et Destructeurs
	Attack();
	Attack(int nbr_frames, std::vector< sf::IntRect > zones_collision, Character* emitter, int damages);
	~Attack();
	
	// Accesseurs
	int getDamages();
	int getActualFrame();
	int getNbrFrames();

	sf::IntRect getCurZone();
	Character* getEmitter();
	std::vector< Character* > getMetTargets();


	// Methodes
	bool update();
	
private:

	int m_damages;
	int m_nbr_frames;
	int m_actual_frame;

	
	bool m_push_effect;
	int m_push_percent;

	std::vector< sf::IntRect > m_zones_collision;

	Character *m_emitter;
	std::vector< Character* > m_met_targets;
};




// Class Character

class Character : public CollisionableEntity
{
public:
	enum State { ATTACKING, MOVING, STANDING, DEFAULT_STATE };

	//Constructeurs et Destructeurs
	Character();
	Character(std::string id, sf::Texture* texture, Map* map);
	Character(std::string id, std::string file_name, sf::Texture* texture, Map* map);
	Character(std::string id, std::string file_name, ResourcesManager* resources_manager, Map* map);

	virtual ~Character();

	//Getters
	int 						getHealth();
	int 						getSpeed();
	State 						getState() const;
	sf::Clock 					getClock();
	sf::Time 					getLastTimeAttack();
	std::vector< Character* > 	getAvTargets();

	// Setters
	void 						setTexture(sf::Texture* texture);
	void 						setHealth(int health);
	void 						setSpeed(int speed);
	void 						setState(State state);
	void 						setLastTimeAttack(sf::Time last_time_attack);
	void 						addAvTarget(Character* target);


	// Methods
	void 						update();
	void 						updateAttack();
	void 						getDrawn(sf::RenderWindow* window);
	void 						move(sf::Vector2f movement);
	void 						move(int mov_x, int mov_y);
	void 						attack();
	void 						takeDamages(int damages);
	void 						drawPart(sf::RenderWindow* window, unsigned int height);
	void 						drawPartAndAbove(sf::RenderWindow* window, unsigned int height);


protected:
	virtual void 				readFeaturesFromString(std::string string);
	virtual void 				readAnimationFromString(std::string string);


private:
	virtual void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;

	
	int 																		m_health;					// Health of a character entity
	int 																		m_speed;					// Speed of a character entity
	std::map< std::pair<Character::State, Sense> , AnimationParameters > 		m_animation_parameters;		// Animation parameters of a character entity identified with the state and the sense of entity
	State 																		m_state;
	sf::Clock 																	m_clock;
	sf::Time 																	m_last_time_attack;
	Attack* 																	m_actual_attack;
	std::vector< Character* > 													m_av_targets;
};




#endif
