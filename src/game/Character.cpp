#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"

#define LINE_SIZE 2000





// ------------------------------------------- Attack -------------------------------------------------//


Attack::Attack()
{
	m_push_effect = true;
	m_push_percent = 10;
	m_actual_frame=0;
	m_nbr_frames = 1;
	m_emitter = NULL;
}

Attack::Attack(int nbr_frames, std::vector< sf::IntRect > zones_collision, Character* emitter, int damages)
{
	m_push_effect = true;
	m_push_percent = 10;
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

	//Met à jour l'état de l'attaque (fait passer à la frame suivante ou termine l'attaque). Renvoie true si l'attaque est finie
	if(this->getEmitter()!=NULL)
	{
		for(unsigned int i=0; i<this->getEmitter()->getAvTargets().size(); i++)
		{
			bool already_met=false;
			for(unsigned int j=0; j<this->getMetTargets().size(); j++)
			{
				if(this->getEmitter()->getAvTargets()[i]==this->getMetTargets()[j])
					already_met=true;
			}

			if(collision_rects(this->getEmitter()->getAvTargets()[i]->getAbsHitbox(), this->getCurZone()))
			{
				//Effet de poussée en fonction de la distance emetteur-receveur et un facteur de pourcentage de poussée
				if(m_push_effect)
				{
					int dist_x = (this->getEmitter()->getAvTargets()[i]->getPosition().x + this->getEmitter()->getAvTargets()[i]->getSize().x/2) - (this->getEmitter()->getPosition().x + this->getEmitter()->getSize().x/2);
					int dist_y = (this->getEmitter()->getAvTargets()[i]->getPosition().y + this->getEmitter()->getAvTargets()[i]->getSize().y/2) - (this->getEmitter()->getPosition().y + this->getEmitter()->getSize().y/2);
					
					
					this->getEmitter()->getAvTargets()[i]->move( sf::Vector2f( m_push_percent*dist_x/100 ,  m_push_percent*dist_y/100 ) );
				}

				if(!already_met)
				{
					this->getEmitter()->getAvTargets()[i]->takeDamages(this->getDamages());
					m_met_targets.push_back(this->getEmitter()->getAvTargets()[i]);
				}
			}
		}
	}


	m_actual_frame++;
	
	if(m_actual_frame % m_nbr_frames==0)
		return true;
	
	return false;
	
}




// ------------------------------------------- Character -------------------------------------------------//



// Constructors and Destructors

Character::Character()
{
	/* Constructor */
	m_type="Character";

	m_actual_attack=NULL;
	m_sprite=NULL;

	m_affiliated_to_map = false;
}


Character::Character(std::string id, sf::Texture* texture, Map* map)
{
	/* Constructor */

	m_id = id;
	m_type="Character";

	m_actual_attack=NULL;
	m_ground_zone = 14;

	m_affiliated_to_map = true;

	m_sprite = new AnimatedSpriteInMap(texture, sf::Vector2f(22, 28), 6, sf::Vector2f(0,30), this->getGroundZone(), map);
	m_sprite->setFPSQuotient(4);

	this->setSize(sf::Vector2f(22, 28));

	this->setState(STANDING);
	this->setSense(DOWN);


	this->update();
}


Character::Character(std::string id, std::string file_name, sf::Texture* texture, Map* map)
{
	/* Constructor */

	m_id = id;
	m_type="Character";

	m_actual_attack=NULL;
	m_sprite=NULL;

	if(this->loadFromFile(file_name, texture) < 0) throw std::string("An error occured while loading from file the Character entity") ;


	// Check if there is default animation parameters or not and create consequently the sprite
	std::map<std::pair<Character::State, Sense>, AnimationParameters>::iterator ptr_animation_parameters;
	if( (ptr_animation_parameters = m_animation_parameters.find(std::pair<Character::State, Sense>(Character::DEFAULT_STATE, DEFAULT_SENSE))) != m_animation_parameters.end() )
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


Character::Character(std::string id, std::string file_name, ResourcesManager* resources_manager, Map* map)
{
	/* Constructor */

	m_id = id;
	m_type="Character";

	m_actual_attack=NULL;
	m_sprite=NULL;

	if(this->loadFromFile(file_name, resources_manager) < 0) throw std::string("An error occured while loading from file the Character entity") ;


	// Check if there is default animation parameters or not and create consequently the sprite
	std::map<std::pair<Character::State, Sense>, AnimationParameters>::iterator ptr_animation_parameters;
	if( (ptr_animation_parameters = m_animation_parameters.find(std::pair<Character::State, Sense>(Character::DEFAULT_STATE, DEFAULT_SENSE))) != m_animation_parameters.end() )
	{
		AnimationParameters def_param = ptr_animation_parameters->second;
		m_sprite = new AnimatedSpriteInMap(m_texture, this->getSize(), def_param.nbr_frames, def_param.init_text_pos, this->getGroundZone(), map);
	}
	else
		m_sprite = new AnimatedSpriteInMap(m_texture, this->getSize(), 1, sf::Vector2f(0,0), this->getGroundZone(), map);


	m_affiliated_to_map = true;


	m_sprite->setFPSQuotient(SITL_FPS_QUOTIENT);

	this->setState(STANDING);
	this->setSense(DOWN);

	this->update();
}



Character::~Character()
{
	/* Destructor */
	if(m_sprite != NULL)
	{
		if(this->isAffiliatedToMap())
			delete ((AnimatedSpriteInMap*)(m_sprite));
		else
			delete m_sprite;
	}

	if(m_actual_attack != NULL)
		delete m_actual_attack;
}



// Getters

int Character::getHealth()
{
	/* Return health of the character */
	return m_health;
}


int Character::getSpeed()
{
	/* Return speed of the character */
	return m_speed;
}


Character::State Character::getState() const
{
	/* Return state of the character */
	return m_state;
}


sf::Clock Character::getClock()
{
	/* Return the clock of the character */
	return m_clock;
}


sf::Time Character::getLastTimeAttack()
{
	/* Return the last time when the character attacked */
	return m_last_time_attack;
}


std::vector< Character* > Character::getAvTargets()
{
	/* Return the potential targets of the character */
	return m_av_targets;
}


// Setters

void Character::setTexture(sf::Texture* texture)
{
	m_texture = texture;

	if(m_sprite != NULL)
		m_sprite->setTexture(m_texture);
}


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

void Character::setState(State state)
{
	// Modifie l'état du personnage en fonction de l'état en paramètre
	m_state = state;	
}


void Character::setLastTimeAttack(sf::Time last_time_attack)
{
	// Modifie le temps de la dernière attaque
	m_last_time_attack = last_time_attack;
}


void Character::addAvTarget(Character* target)
{
	// Rajoute une cible potentielle pouvant être attaquée par le personnage tout en vérifiant si celle ci n'est pas déjà dans la liste des cibles potentielles
	bool already_av =false;
	for(unsigned int i=0; i<this->getAvTargets().size(); i++)
	{
		if(this->getAvTargets()[i]==target)
			already_av=true;
	}
	
	if(!already_av)
		m_av_targets.push_back(target);
}



// Methods

void Character::update()
{
	/* Update sprite state and position depending on the character entity */
	if(m_sprite!=NULL)
	{
		std::map<std::pair<Character::State, Sense>, AnimationParameters>::iterator ptr_animation_parameters;

		if(	(ptr_animation_parameters = m_animation_parameters.find(std::pair<Character::State, Sense>(this->getState(), this->getSense()))) != m_animation_parameters.end() )
		{
			// Change the sprite animation of the Collector entity depending on its sense and state
			AnimationParameters parameter = ptr_animation_parameters->second;
			m_sprite->setParameters(parameter);
		}
		else if( (ptr_animation_parameters = m_animation_parameters.find(std::pair<Character::State, Sense>(Character::DEFAULT_STATE, DEFAULT_SENSE))) != m_animation_parameters.end() )
		{
			// Set the default animation if there is one
			AnimationParameters parameter = ptr_animation_parameters->second;
			m_sprite->setParameters(parameter);
		}

		
		// Met à jour le sprite
		m_sprite->setPosition(m_position);

		if(this->isAffiliatedToMap())
			((AnimatedSpriteInMap*)(m_sprite))->update();
		else
			m_sprite->update();
	}	
}



void Character::updateAttack()
{
	/* Check the state of the attack and update it consequently */
	if(m_state==ATTACKING && m_actual_attack!=NULL)
	{
		if(m_actual_attack->update()) // If the attack is finished
		{
			m_state=STANDING;
			m_last_time_attack = m_clock.getElapsedTime();

			delete m_actual_attack;
			m_actual_attack=NULL;
		}
	}
}


void Character::move(sf::Vector2f movement)
{
	/* Move the character */
	m_position+=movement;
	
	if(m_sprite!=NULL)
		m_sprite->setPosition(m_position);
}	


void Character::move(int mov_x, int mov_y)
{
	/* Move the character */
	m_position+=sf::Vector2f (mov_x, mov_y);

	if(m_sprite!=NULL)
		m_sprite->setPosition(m_position);
}	


void Character::attack()
{
	/* Launch an attack if the character doesn't attack */
	if(m_state!=ATTACKING && m_actual_attack==NULL && (m_clock.getElapsedTime().asMilliseconds() - m_last_time_attack.asMilliseconds() > 250 ) )
	{
		//Exemple with a linear attack
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
	/* Take damages depending on the value in parameter */
	m_health-=damages;
}

		
void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the character */

	if(m_sprite!=NULL)
		target.draw(*m_sprite, states);

	if(this->getState()==ATTACKING)
	{
		sf::RectangleShape test_rect(sf::Vector2f(m_actual_attack->getCurZone().width, m_actual_attack->getCurZone().height));
		test_rect.setPosition(m_actual_attack->getCurZone().left, m_actual_attack->getCurZone().top);
		target.draw(test_rect);
	}
};


void Character::drawPart(sf::RenderWindow* window, unsigned int height)
{
	/* Draw part of the character entity in the chosen height */
	if(m_sprite != NULL && this->isAffiliatedToMap())
		((AnimatedSpriteInMap*)(m_sprite))->drawPart(window, height);
}


void Character::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{

	/* Draw part of the character entity in the chosen height and everything above */
	if(m_sprite != NULL && this->isAffiliatedToMap())
		((AnimatedSpriteInMap*)(m_sprite))->drawPartAndAbove(window, height);
}



void Character::readFeaturesFromString(std::string string)
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

		std::cout << hitbox.left << std::endl;
		hitbox.top = atoi(tag=strtok(NULL, " ,"));
		std::cout << hitbox.top << std::endl;
		hitbox.width = atoi(tag=strtok(NULL, " ,"));	

		std::cout << hitbox.width << std::endl;
		hitbox.height = atoi(tag=strtok(NULL, " )"));

		std::cout << hitbox.height << std::endl;
		if(hitbox.left!=-1 && hitbox.top!=-1 && hitbox.width!=-1 && hitbox.height!=-1) this->setHitbox(hitbox);
	}
	else if(strstr(string.c_str(), "Speed : ")!=NULL && tag!=NULL) this->setSpeed(atoi(tag));
	else if(strstr(string.c_str(), "Health : ")!=NULL && tag!=NULL) this->setHealth(atoi(tag));

}


void Character::readAnimationFromString(std::string string)
{

	char line[LINE_SIZE];
	strcpy(line, string.c_str());

	strtok(line, "{\n");
	char* tag = strtok(NULL, "}\n");
	

	// default
	if(strstr(string.c_str(), "default")!=NULL && tag!=NULL)
	{	
		AnimationParameters anim_prmtrs = animationParametersFromString(std::string(tag));

		if(anim_prmtrs.size.x == -1 || anim_prmtrs.size.y == -1 || anim_prmtrs.init_text_pos.x == -1 || anim_prmtrs.init_text_pos.y == -1 || anim_prmtrs.spacing == -1 || anim_prmtrs.nbr_frames == -1) {std::cerr << "An error occured while loading animation parameters" << std::endl; return;}
		
		m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::DEFAULT_STATE, DEFAULT_SENSE), anim_prmtrs) );
	}
	else if(strstr(string.c_str(), "moving")!=NULL && tag!=NULL)
	{	
		AnimationParameters anim_prmtrs = animationParametersFromString(std::string(tag));

		if(anim_prmtrs.size.x == -1 || anim_prmtrs.size.y == -1 || anim_prmtrs.init_text_pos.x == -1 || anim_prmtrs.init_text_pos.y == -1 || anim_prmtrs.spacing == -1 || anim_prmtrs.nbr_frames == -1) {std::cerr << "An error occured while loading animation parameters" << std::endl; return;}

		if(strstr(string.c_str(), "upleft")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, UP_LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "upright")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, UP_RIGHT), anim_prmtrs) );
		else if(strstr(string.c_str(), "downleft")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, DOWN_LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "downright")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, DOWN_RIGHT), anim_prmtrs) );
		else if(strstr(string.c_str(), "up")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, UP), anim_prmtrs) );
		else if(strstr(string.c_str(), "down")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, DOWN), anim_prmtrs) );
		else if(strstr(string.c_str(), "left")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "right")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::MOVING, RIGHT), anim_prmtrs) );
	}
	else if(strstr(string.c_str(), "standing")!=NULL && tag!=NULL)
	{	
		AnimationParameters anim_prmtrs = animationParametersFromString(std::string(tag));

		if(anim_prmtrs.size.x == -1 || anim_prmtrs.size.y == -1 || anim_prmtrs.init_text_pos.x == -1 || anim_prmtrs.init_text_pos.y == -1 || anim_prmtrs.spacing == -1 || anim_prmtrs.nbr_frames == -1) {std::cerr << "An error occured while loading animation parameters" << std::endl; return;}
		
		if(strstr(string.c_str(), "upleft")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, UP_LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "upright")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, UP_RIGHT), anim_prmtrs) );
		else if(strstr(string.c_str(), "downleft")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, DOWN_LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "downright")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, DOWN_RIGHT), anim_prmtrs) );
		else if(strstr(string.c_str(), "up")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, UP), anim_prmtrs) );
		else if(strstr(string.c_str(), "down")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, DOWN), anim_prmtrs) );
		else if(strstr(string.c_str(), "left")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING, LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "right")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::STANDING , RIGHT), anim_prmtrs) );	
	}
	else if(strstr(string.c_str(), "attacking")!=NULL && tag!=NULL)
	{	
		AnimationParameters anim_prmtrs = animationParametersFromString(std::string(tag));

		if(anim_prmtrs.size.x == -1 || anim_prmtrs.size.y == -1 || anim_prmtrs.init_text_pos.x == -1 || anim_prmtrs.init_text_pos.y == -1 || anim_prmtrs.spacing == -1 || anim_prmtrs.nbr_frames == -1) {std::cerr << "An error occured while loading animation parameters" << std::endl; return;}
		
		if(strstr(string.c_str(), "upleft")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, UP_LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "upright")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, UP_RIGHT), anim_prmtrs) );
		else if(strstr(string.c_str(), "downleft")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, DOWN_LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "downright")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, DOWN_RIGHT), anim_prmtrs) );
		else if(strstr(string.c_str(), "up")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, UP), anim_prmtrs) );
		else if(strstr(string.c_str(), "down")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, DOWN), anim_prmtrs) );
		else if(strstr(string.c_str(), "left")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING, LEFT), anim_prmtrs) );
		else if(strstr(string.c_str(), "right")!=NULL)
			m_animation_parameters.insert( std::pair< std::pair< Character::State, Sense > , AnimationParameters > ( std::pair< Character::State, Sense >(Character::ATTACKING , RIGHT), anim_prmtrs) );	
	}
}
