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
	// Renvoie les dommages qu'est censé faire passer l'attaque
	return m_damages;
}

int Attack::getActualFrame()
{
	// Renvoie la frame actuelle de l'attaque
	return m_actual_frame;
}

int Attack::getNbrFrames()
{
	// Renvoie le nombre de frames de l'attaque
	return m_nbr_frames;
}

sf::IntRect Attack::getCurZone()
{
	//Renvoie la hitbox correspondant à la frame d'attaque actuelle, en tenant compte de la frame, du sens et de la position de l'emetteur (s'il y a emetteur) 

	sf::IntRect actual_zone(m_zones_collision[m_actual_frame]);
	sf::IntRect res_zone=actual_zone;
	float angle(atan2(actual_zone.left, actual_zone.top)), 
	  length(sqrt((actual_zone.left) * (actual_zone.left) + (actual_zone.top) * (actual_zone.top)));
	sf::Vector2f e_ctr(m_emitter->getAbsHitbox().left + m_emitter->getAbsHitbox().width/2, m_emitter->getAbsHitbox().top + m_emitter->getAbsHitbox().height/2);

	
	if(m_emitter!=NULL)
	{
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
	}

	return res_zone;
}

Character* Attack::getEmitter()
{
	// Renvoie le pointeur vers l'emetteur de l'attaque
	return m_emitter;
}

std::vector< Character* > Attack::getMetTargets()
{
	// Renvoie un vecteur contenant les pointeurs vers les cibles déjà rencontrées (utiles pour que la cible ne subisse qu'une fois l'attaque)
	return m_met_targets;
}

bool Attack::update()
{

	//Met à jour l'état de l'attaque (fait passer à la frame suivante ou termine l'attaque)    	
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
	m_sprite.setSize(sf::Vector2f(22, 28));
	m_sprite.setPosition(rect_sprite.left, rect_sprite.top);

	m_sprite.setFillColor(color);

	m_sprite2 = new AnimatedSprite(texture, sf::Vector2f(22, 28), 6, sf::Vector2f(0,30) );
	m_sprite2->setFPSQuotient(4);

	this->setSize(sf::Vector2f(22, 28));
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
	// Renvoie la santé d'un personnage
	return m_health;
}

int Character::getSpeed()
{
	// Renvoie la vitesse de déplacement d'un personnage
	return m_speed;
}

sf::Vector2f Character::getPosition()
{
	// Renvoie la position d'un personnage
	return m_position;
}

sf::Vector2f Character::getSize()
{
	// Renvoie un vecteur de 2 dimensions contenant la taille d'un personnage en largeur et en longueur
	return m_size;
}

sf::IntRect Character::getHitbox()
{
	// Renvoie la hitbox (dont la taille diffère de celle du personnage) relative au personnage (la position est relative à celle du personnage)
	return m_hitbox;
}

sf::IntRect Character::getAbsHitbox()
{
	// Renvoie la hitbox absolue (la position est absolue)
	return sf::IntRect(m_hitbox.left + m_position.x + m_size.x/2 - m_hitbox.width/2, m_hitbox.top + m_position.y + m_size.y/2 - m_hitbox.height/2, 
			   m_hitbox.width, m_hitbox.height);
}

State Character::getState()
{
	// Renvoie l'état du personnage
	return m_state;
}

Sense Character::getSense()
{
	// Renvoie le sens du personnage
	return m_sense;
}

std::vector< Character* > Character::getAvTargets()
{
	// Renvoie les cibles potentielles pouvant être attaquées par le personnage
	return m_av_targets;
}

//Modificateurs

void Character::setHealth(int health)
{
	// Modifie la santé du personnage	
	m_health = health;
}

void Character::setSpeed(int speed)
{
	// Modifie la vitesse de déplacement du personnage
	m_speed = speed;
}

void Character::setPosition(sf::Vector2f position)
{

	//Modifie la position du personnage
	m_position = position;
	m_sprite.setPosition(m_position);
	
	m_sprite2->setPosition(m_position);
}

void Character::setSize(sf::Vector2f size)
{	
	//Modifie la taille du personnage
	m_size = size;
}

void Character::setHitbox(sf::IntRect hitbox)
{
	// Modifie la hitbox du personnage (la position est relative à celle du personnage)
	m_hitbox = hitbox;
}

void Character::setState(State state)
{
	// Modifie l'état du personnage en fonction de l'état en paramètre
	m_state = state;	
}

void Character::setSense(Sense sense)
{
	// Modifie le sens du personnage en fonction du sens en paramètre
	m_sense = sense;
}

void Character::addAvTarget(Character* target)
{
	// Rajoute une cible potentielle pouvant être attaquée par le personnage tout en vérifiant si celle ci n'est pas déjà dans la liste des cibles potentielles
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
	//Dessine le personnage sur une fenêtre dont le pointeur est pris en paramètre
	
	window->draw(m_sprite);
	window->draw(*m_sprite2);

	if(this->getState()==ATTACKING)
	{
		sf::RectangleShape test_rect(sf::Vector2f(m_actual_attack->getCurZone().width, m_actual_attack->getCurZone().height));
		test_rect.setPosition(m_actual_attack->getCurZone().left, m_actual_attack->getCurZone().top);
		window->draw(test_rect);
	}
}



void Character::move(sf::Vector2f movement)
{
	// Déplace le personnage
	m_position+=movement;
	m_sprite2->setPosition(m_position);
	
}	

void Character::attack()
{
	
	// Lance une attaque si le personnage n'attaque déjà pas
	if(m_state!=ATTACKING && m_actual_attack==NULL)
	{
		//Exemple d'attaque rectiligne
		std::vector< sf::IntRect > test_rect;
		for(int i=0; i<this->getSize().x; i+=4)
		{
			test_rect.push_back(sf::IntRect(i-this->getSize().x/2,this->getSize().y/2+5,10,10));
		}

		m_actual_attack = new Attack(test_rect.size(), test_rect, this, 2);
		m_state=ATTACKING;

	}
}

void Character::takeDamages(int damages)
{
	//Prends des dommages en fonction des dégâts en paramètres.
	m_health-=damages;
}
