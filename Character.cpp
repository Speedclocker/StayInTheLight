#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"







// ------------------------------------------- Attack -------------------------------------------------//


Attack::Attack()
{
    m_actual_frame=0;
    m_nbr_frames = 1;
    m_emitter = NULL;
}

Attack::Attack(int nbr_frames, std::vector< sf::IntRect > zones_collision, Character* emitter, int damages)
{

	m_actual_frame=0;
	m_nbr_frames = nbr_frames;
	m_zones_collision = zones_collision;
	m_emitter = emitter;
	m_damages = damages;
    
}

Attack::~Attack()
{
  
}

int Attack::getDamages()
{
	return m_damages;
}

int Attack::getActualFrame()
{
    return m_actual_frame;
}

int Attack::getNbrFrames()
{
    return m_nbr_frames;
}

sf::IntRect Attack::getCurZone()
{
    sf::IntRect actual_zone(m_zones_collision[m_actual_frame]);
    sf::IntRect res_zone=actual_zone;
    float angle(atan2(actual_zone.left, actual_zone.top)), 
          length(sqrt((actual_zone.left) * (actual_zone.left) + (actual_zone.top) * (actual_zone.top)));
    sf::Vector2f e_ctr(m_emitter->getAbsHitbox().left + m_emitter->getAbsHitbox().width/2, m_emitter->getAbsHitbox().top + m_emitter->getAbsHitbox().height/2);
    
    switch(m_emitter->getSense())
    {
        case(UP):
            res_zone.left = e_ctr.x + sin(angle+PI)*length - actual_zone.width/2;
            res_zone.top = e_ctr.y + cos(angle+PI)*length - actual_zone.height/2;
            res_zone.width = actual_zone.width;
            res_zone.height = actual_zone.height;
            break;
            
        case(LEFT):
            res_zone.left = e_ctr.x + sin(angle-PI/2)*length - actual_zone.height/2;
            res_zone.top = e_ctr.y + cos(angle-PI/2)*length - actual_zone.width/2;
            res_zone.width = actual_zone.height;
            res_zone.height = actual_zone.width;
            break; 
            
        case(RIGHT):
            res_zone.left = e_ctr.x + sin(angle+PI/2)*length - actual_zone.height/2;
            res_zone.top = e_ctr.y + cos(angle+PI/2)*length - actual_zone.width/2;
            res_zone.width = actual_zone.height;
            res_zone.height = actual_zone.width;
            break; 
            
        case(DOWN_RIGHT):
            res_zone.left = e_ctr.x + sin(angle+PI/4)*length - actual_zone.width/2;
            res_zone.top = e_ctr.y + cos(angle+PI/4)*length - actual_zone.height/2;
            res_zone.width = actual_zone.width;
            res_zone.height = actual_zone.height;
            break;
            
        case(DOWN_LEFT):
            res_zone.left = e_ctr.x + sin(angle-PI/4)*length - actual_zone.width/2;
            res_zone.top = e_ctr.y + cos(angle-PI/4)*length - actual_zone.height/2;
            res_zone.width = actual_zone.width;
            res_zone.height = actual_zone.height;
            break; 
            
        case(UP_RIGHT):
            res_zone.left = e_ctr.x + sin(angle+3*PI/4)*length - actual_zone.width/2;
            res_zone.top = e_ctr.y + cos(angle+3*PI/4)*length - actual_zone.height/2;
            res_zone.width = actual_zone.width;
            res_zone.height = actual_zone.height;
            break;
            
        case(UP_LEFT):
            res_zone.left = e_ctr.x + sin(angle-3*PI/4)*length - actual_zone.width/2;
            res_zone.top = e_ctr.y + cos(angle-3*PI/4)*length - actual_zone.height/2;
            res_zone.width = actual_zone.width;
            res_zone.height = actual_zone.height;
            break; 
            
        default:
            res_zone.left = e_ctr.x + sin(angle)*length - actual_zone.width/2;
            res_zone.top = e_ctr.y + cos(angle)*length - actual_zone.height/2;
            res_zone.width = actual_zone.width;
            res_zone.height = actual_zone.height;
            break;
    }
    
    return res_zone;
}

Character* Attack::getEmitter()
{
	return m_emitter;
}

std::vector< Character* > Attack::getMetTargets()
{
	return m_met_targets;
}

bool Attack::update()
{

    	
	if(this->getEmitter()!=NULL)
	{
		for(int i=0; i<this->getEmitter()->getAvTargets().size(); i++)
		{
			bool already_met=false;
			for(int j=0; j<this->getMetTargets().size(); j++)
			{
				if(this->getEmitter()->getAvTargets()[i]==this->getMetTargets()[j])
					already_met=true;
			}

			if(!already_met && collision_rects(this->getEmitter()->getAvTargets()[i]->getAbsHitbox(), this->getCurZone()))
			{
				this->getEmitter()->getAvTargets()[i]->takeDamages(this->getDamages());
				m_met_targets.push_back(this->getEmitter()->getAvTargets()[i]);
			}
		}
	}


	m_actual_frame++;
	
	if(m_actual_frame % m_nbr_frames==0)
		return true;
	
	return false;
	
}










// ------------------------------------------- Character -------------------------------------------------//





Character::Character()
{
    
}

Character::Character(sf::Texture* texture, sf::IntRect rect_sprite, sf::Color color)
{
	m_actual_attack=NULL;
	m_sprite.setSize(sf::Vector2f(rect_sprite.width, rect_sprite.height));
	m_sprite.setPosition(rect_sprite.left, rect_sprite.top);

	m_sprite.setFillColor(color);

	m_sprite2 = new AnimatedSprite(texture, sf::Vector2f(22, 28), 8, sf::Vector2f(0,30) );
	m_sprite2->setFPSQuotient(4);

	this->setSize(sf::Vector2f(rect_sprite.width, rect_sprite.height));
	this->update();

	this->setState(STANDING);
	
	this->setSense(DOWN);
}

Character::~Character()
{
    //delete m_actual_attack;
}


//Accesseurs


int Character::getHealth()
{
    return m_health;
}

int Character::getSpeed()
{
    return m_speed;
}

sf::Vector2f Character::getPosition()
{
    return m_position;
}

sf::Vector2f Character::getSize()
{
    return m_size;
}

sf::IntRect Character::getHitbox()
{
    return m_hitbox;
}

sf::IntRect Character::getAbsHitbox()
{
    return sf::IntRect(m_hitbox.left + m_position.x, m_hitbox.top + m_position.y, m_hitbox.width, m_hitbox.height);
}

State Character::getState()
{
    return m_state;
}

Sense Character::getSense()
{
    return m_sense;
}

std::vector< Character* > Character::getAvTargets()
{
	return m_av_targets;
}

//Modificateurs

void Character::setHealth(int health)
{
    m_health = health;
}

void Character::setSpeed(int speed)
{
    m_speed = speed;
}

void Character::setPosition(sf::Vector2f position)
{
    m_position = position;
    this->update();
}

void Character::setSize(sf::Vector2f size)
{
    m_size = size;
}

void Character::setHitbox(sf::IntRect hitbox)
{
    m_hitbox = hitbox;
}

void Character::setState(State state)
{
    m_state = state;
}

void Character::setSense(Sense sense)
{
    m_sense = sense;
}

void Character::addAvTarget(Character* target)
{
	bool already_av =false;
	for(int i=0; i<this->getAvTargets().size(); i++)
	{
		if(this->getAvTargets()[i]==target)
			already_av=true;
	}
	
	if(!already_av)
    		m_av_targets.push_back(target);
}





//Méthodes

void Character::update()
{
    //MAJ Position du sprite en fonction de l'entité Character
    m_sprite.setPosition(m_position);
	
    m_sprite2->setPosition(m_position);
    m_sprite2->update();
    

    //Vérifie l'état de l'attaque
    

	if(m_state==ATTACKING && m_actual_attack!=NULL)
	{
		if(m_actual_attack->update())
		{
			m_state=STANDING;

			delete m_actual_attack;
			m_actual_attack=NULL;
		}
	}

    
}


void Character::getDrawn(sf::RenderWindow* window)
{
    if(this->getState()==ATTACKING)
    {
	sf::RectangleShape test_rect(sf::Vector2f(m_actual_attack->getCurZone().width, m_actual_attack->getCurZone().height));
	test_rect.setPosition(m_actual_attack->getCurZone().left, m_actual_attack->getCurZone().top);
	window->draw(test_rect);
    }
    window->draw(m_sprite);
    window->draw(*m_sprite2);
}



void Character::move(sf::Vector2f movement)
{
	m_position+=movement;
	m_sprite2->setPosition(m_position);
	
}

void Character::attack()
{
    
    if(m_state!=ATTACKING && m_actual_attack==NULL)
    {
                
        std::vector< sf::IntRect > test_rect;
	for(int i=0; i<this->getSize().x; i+=4)
	{
        	test_rect.push_back(sf::IntRect(i-this->getSize().x/2,this->getSize().y/2+15,10,10));
        }
        
        m_actual_attack = new Attack(test_rect.size(), test_rect, this, 2);
        m_state=ATTACKING;
    
    }
}

void Character::takeDamages(int damages)
{
	m_health-=damages;
}
