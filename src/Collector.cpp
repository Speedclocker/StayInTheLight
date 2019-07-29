#include <SFML/Graphics.hpp>
#include <iostream>
#include "Collector.h"


#define LINE_SIZE 2000



// ------------------------------------------- Collector -------------------------------------------------//



Collector::Collector()
{
	m_type="Collector";

	m_sprite=NULL;

	m_affiliated_to_map = false;
}


Collector::Collector(sf::Texture* texture, Map* map)
{
	m_type="Collector";

	m_ground_zone = 14;

	m_sprite = new AnimatedSpriteInMap(texture, sf::Vector2f(22, 28), 6, sf::Vector2f(0,30), this->getGroundZone(), map);

	m_affiliated_to_map = true;

	m_sprite->setFPSQuotient(4);

	this->setSize(sf::Vector2f(22, 28));

	this->setState(Collector::STANDING);
	this->setSense(DOWN);

	this->update();

}


Collector::Collector(std::string file_name, sf::Texture* texture, Map* map)
{
	/* Constructor */
	m_type="Collector";

	m_sprite=NULL;

	if(this->loadFromFile(file_name, texture) < 0) throw std::string("An error occured while loading from file the Collector entity");

	// Check if there is default animation parameters or not and create consequently the sprite
	std::map<std::pair<Collector::State, Sense>, AnimationParameters>::iterator ptr_animation_parameters;
	if( (ptr_animation_parameters = m_animation_parameters.find(std::pair<Collector::State, Sense>(Collector::DEFAULT_STATE, DEFAULT_SENSE))) != m_animation_parameters.end() )
	{
		AnimationParameters def_param = ptr_animation_parameters->second;
		m_sprite = new AnimatedSpriteInMap(texture, this->getSize(), def_param.nbr_frames, def_param.init_text_pos, this->getGroundZone(), map);
	}
	else
		m_sprite = new AnimatedSpriteInMap(texture, this->getSize(), 1, sf::Vector2f(0,0), this->getGroundZone(), map);

	m_affiliated_to_map = true;

	m_sprite->setFPSQuotient(SITL_FPS_QUOTIENT);

	this->setState(STANDING);
	this->setSense(DOWN);

	this->update();
}



Collector::~Collector()
{
	std::cout << "Del Collector" << std::endl;
	if(m_sprite !=NULL)
	{
		if(this->isAffiliatedToMap())
			delete ((AnimatedSpriteInMap*)(m_sprite));
		else
			delete m_sprite;
	}
}



// Getters

int Collector::getHealth()
{
	// Renvoie la santé d'un personnage
	return m_health;
}

int Collector::getSpeed()
{
	// Renvoie la vitesse de déplacement d'un personnage
	return m_speed;
}

sf::IntRect Collector::getHitbox()
{
	// Renvoie la hitbox (dont la taille diffère de celle du personnage) relative au personnage (la position est relative à celle du personnage)
	return m_hitbox;
}

sf::IntRect Collector::getAbsHitbox()
{
	// Renvoie la hitbox absolue (la position est absolue)
	return sf::IntRect(m_hitbox.left + m_position.x, m_hitbox.top + m_position.y, m_hitbox.width, m_hitbox.height);

}

Collector::State Collector::getState() const
{
	// Renvoie l'état du personnage
	return m_state;
}

sf::Clock Collector::getClock()
{
	// Renvoie l'horloge du personnage
	return m_clock;
}



// Setters

void Collector::setTexture(sf::Texture* texture)
{
	m_texture = texture;	
	
	if(m_sprite != NULL)
		m_sprite->setTexture(m_texture);
}


void Collector::setHealth(int health)
{
	// Modifie la santé du personnage	
	m_health = health;
}


void Collector::setSpeed(int speed)
{
	// Modifie la vitesse de déplacement du personnage
	m_speed = speed;
}


void Collector::setHitbox(sf::IntRect hitbox)
{
	// Modifie la hitbox du personnage (la position est relative à celle du personnage)
	m_hitbox = hitbox;
}


void Collector::setState(Collector::State state)
{
	// Modifie l'état du personnage en fonction de l'état en paramètre
	m_state = state;	
}



// Methodes

void Collector::update()
{
	// Update the Collector entity

	if(m_sprite!=NULL)
	{
		std::map<std::pair<Collector::State, Sense>, AnimationParameters>::iterator ptr_animation_parameters;

		if(	(ptr_animation_parameters = m_animation_parameters.find(std::pair<Collector::State, Sense>(this->getState(), this->getSense()))) != m_animation_parameters.end() )
		{
			// Change the sprite animation of the Collector entity depending on its sense and state
			AnimationParameters parameter = ptr_animation_parameters->second;
			m_sprite->setParameters(parameter.size, parameter.nbr_frames, parameter.init_text_pos, parameter.spacing);
		}
		else if( (ptr_animation_parameters = m_animation_parameters.find(std::pair<Collector::State, Sense>(Collector::DEFAULT_STATE, DEFAULT_SENSE))) != m_animation_parameters.end() )
		{
			// Set the default animation if there is one
			AnimationParameters parameter = ptr_animation_parameters->second;
			m_sprite->setParameters(parameter.size, parameter.nbr_frames, parameter.init_text_pos, parameter.spacing);
		}

		// Update the sprite position
		m_sprite->setPosition(m_position);

		if(this->isAffiliatedToMap())
			((AnimatedSpriteInMap*)(m_sprite))->update();
		else
			m_sprite->update();
	}	
}


void Collector::move(sf::Vector2f movement)
{
	// Move the Collector entity
	m_position+=movement;
	
	if(m_sprite!=NULL)
		m_sprite->setPosition(m_position);
	
}	



void Collector::move(int mov_x, int mov_y)
{
	// Move the Collector entity
	m_position+=sf::Vector2f (mov_x, mov_y);

	if(m_sprite!=NULL)
		m_sprite->setPosition(m_position);
	
}	


void Collector::takeDamages(int damages)
{
	// Take damages depending on the parameter value
	m_health-=damages;
}



void Collector::drawPart(sf::RenderWindow* window, unsigned int height)
{
	// Draw a part of the entity designed by the chosen height in parameter
	if(m_sprite!=NULL && this->isAffiliatedToMap())
		((AnimatedSpriteInMap*)(m_sprite))->drawPart(window, height);
}



void Collector::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	if(m_sprite!=NULL && this->isAffiliatedToMap())
		((AnimatedSpriteInMap*)(m_sprite))->drawPartAndAbove(window, height);
}



void Collector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw the Collector entity
	if(m_sprite!=NULL)
		target.draw(*m_sprite, states);
}



void Collector::readFeaturesFromString(std::string string)
{
	char line[LINE_SIZE];
	strcpy(line, string.c_str());

	strtok(line, ": \n");
	char* tag = strtok(NULL, ": \n");

	// Hitbox feature
	if(strstr(string.c_str(), "Hitbox : ")!=NULL && tag!=NULL)
	{
		// load from file the Hitbox
		sf::IntRect hitbox = sf::IntRect(-1,-1,-1,-1);

		tag=tag+1;
		hitbox.left = atoi(tag=strtok(tag, " ,"));	
		hitbox.top = atoi(tag=strtok(NULL, " ,"));
		hitbox.width = atoi(tag=strtok(NULL, " ,"));
		hitbox.height = atoi(tag=strtok(NULL, " )"));

		if(hitbox.left!=-1 && hitbox.top!=-1 && hitbox.width!=-1 && hitbox.height!=-1) this->setHitbox(hitbox);
	}
	// Collectible feature
	else if(strstr(string.c_str(), "Collectible : ")!=NULL && tag!=NULL)	m_collectible = atoi(tag);
}


void Collector::readAnimationFromString(std::string string)
{
	char line[LINE_SIZE];
	strcpy(line, string.c_str());

	strtok(line, "{\n");
	char* tag = strtok(NULL, "}\n");

	// default
	if(strstr(string.c_str(), "default : ")!=NULL && tag!=NULL)
	{	
		AnimationParameters anim_prmtrs = animationParametersFromString(std::string(tag));
		if(anim_prmtrs.size.x == -1 || anim_prmtrs.size.y == -1 || anim_prmtrs.init_text_pos.x == -1 || anim_prmtrs.init_text_pos.y == -1 || anim_prmtrs.spacing == -1 || anim_prmtrs.nbr_frames == -1)
			{std::cerr << "An error occured while loading animation parameters" << std::endl; return;}
		
		m_animation_parameters.insert( std::pair< std::pair< State, Sense > , AnimationParameters > ( std::pair< State, Sense >(Collector::DEFAULT_STATE, DEFAULT_SENSE), anim_prmtrs) );
	}
}
