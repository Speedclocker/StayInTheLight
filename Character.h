#ifndef __CHARACTER_H__
#define __CHARACTER_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "collision.h"
#include "Animation.h"
#include "Object.h"


#define PI 3.14159




enum State
{
	ATTACKING, MOVING, STANDING
};

enum Sense
{
	UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT
};


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



class Character : public Object
{
public:
	//Constructeurs et Destructeurs
	Character();
	Character(sf::Texture* texture, sf::IntRect rect_sprite, sf::Color color);
	~Character();

	//Accesseurs
	int getHealth();
	int getSpeed();

	sf::Vector2f getPosition();
	sf::Vector2f getSize();

	AnimatedSprite* getSprite();	
	
	sf::IntRect getHitbox();
	sf::IntRect getAbsHitbox();
	State getState();
	Sense getSense();

	sf::Clock getClock();
	sf::Time getLastTimeAttack();

	std::vector< Character* > getAvTargets();


	//Modificateurs 
	void setHealth(int health);
	void setSpeed(int speed);

	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);

	void setHitbox(sf::IntRect hitbox);

	void setState(State state);
	void setSense(Sense sense);
	
	void setLastTimeAttack(sf::Time last_time_attack);

	void addAvTarget(Character* target);

	//Méthodes
	void update();
	void getDrawn(sf::RenderWindow* window);

	void move(sf::Vector2f movement);
	void attack();
	void takeDamages(int damages);
	
private:
	int m_health;
	int m_speed;


	sf::Vector2f m_position;
	sf::Vector2f m_size;

	sf::RectangleShape m_sprite;
	AnimatedSprite* m_sprite2;
	sf::IntRect m_hitbox;

	State m_state;
	Sense m_sense;

	sf::Clock m_clock;
	sf::Time m_last_time_attack;

	Attack* m_actual_attack;
	std::vector< Character* > m_av_targets;
};




#endif
