#include <SFML/Graphics.hpp>
#include <iostream>
#include "Collector.h"


#define LINE_SIZE 2000

#define STR_SIZE 300


AnimationParameters animationParametersFromString(std::string string)
{
	// Set Animation parameters from string. String format must be : (size.x,size.y),nbr_frames,(init_text_pos.x,init_text_pos.y),spacing

	sf::Vector2f size(-1,-1); 
	int nbr_frames(-1); 
	sf::Vector2f init_text_pos(-1, -1); 
	int spacing(-1);

	char string_chr[STR_SIZE];
	if(string.c_str()[0]=='(') 	strcpy(string_chr, string.c_str()+1);
	else 						strcpy(string_chr, string.c_str());

	char* tmp_buffer;

	if((tmp_buffer = strtok(string_chr, ","))!=NULL) 
		size.x = atoi(tmp_buffer); 

	if((tmp_buffer = strtok(NULL, "),"))!=NULL)
		size.y = atoi(tmp_buffer); 

	if((tmp_buffer = strtok(NULL, ",()"))!=NULL)
		nbr_frames = atoi(tmp_buffer); 

	if((tmp_buffer = strtok(NULL, ",()"))!=NULL)
		init_text_pos.x = atoi(tmp_buffer);

	if((tmp_buffer = strtok(NULL, ",()"))!=NULL)
		init_text_pos.y = atoi(tmp_buffer);

	if((tmp_buffer = strtok(NULL, ",()\n\0"))!=NULL)
		spacing = atoi(tmp_buffer);

	AnimationParameters animation_parameters = { size, nbr_frames, init_text_pos, spacing };

	return animation_parameters;
}


// ------------------------------------------- Collector -------------------------------------------------//



Collector::Collector()
{
	m_type="Collector";

	m_sprite=NULL;
}


Collector::Collector(sf::Texture* texture, Map* map)
{
	m_type="Collector";

	m_ground_zone = 14;

	m_sprite = new AnimatedSpriteInMap(texture, sf::Vector2f(22, 28), 6, sf::Vector2f(0,30), this->getGroundZone(), map);
	m_sprite->setFPSQuotient(4);

	this->setSize(sf::Vector2f(22, 28));

	this->setState(Collector::STANDING);
	this->setSense(DOWN);

	this->update();

}


Collector::~Collector()
{
	if(m_sprite != NULL)
		delete m_sprite;
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
	return sf::IntRect(m_hitbox.left + m_position.x + m_size.x/2 - m_hitbox.width/2, m_hitbox.top + m_position.y + m_size.y/2 - m_hitbox.height/2, 
		m_hitbox.width, m_hitbox.height);
}

Collector::State Collector::getState() const
{
	// Renvoie l'état du personnage
	return m_state;
}

Sense Collector::getSense()
{
	// Renvoie le sens du personnage
	return m_sense;
}

sf::Clock Collector::getClock()
{
	// Renvoie l'horloge du personnage
	return m_clock;
}


AnimatedSpriteInMap* Collector::getSprite()
{
	// Renvoie un pointeur vers le sprite du personnage
	return m_sprite;
}




// Setters

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

void Collector::setSense(Sense sense)
{
	// Modifie le sens du personnage en fonction du sens en paramètre
	m_sense = sense;
}





//Methodes

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

		// Update the sprite position
		m_sprite->setPosition(m_position);
		m_sprite->update();
	}	
}


int Collector::loadFromFile(std::string file_name, sf::Texture* texture)
{
	std::ifstream file_to_load;
	file_to_load.open(file_name);
	if(file_to_load.fail()) {std::cerr << "An error happened while opening the entity file" << file_name << std::endl; file_to_load.close(); return -1; }

	std::string name="";
	std::string type="";
	sf::Vector2f size = sf::Vector2f(-1,-1);
	sf::IntRect hitbox = sf::IntRect(-1,-1,-1,-1);
	std::string texture_file_name="";

	std::string buffer="";


	// Read the file until we reach the end
	while(std::getline(file_to_load, buffer))
	{
		// Header Tag
		if(strstr(buffer.c_str(), "[Header]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Header]")==NULL)
			{
				char line[LINE_SIZE];
				strcpy(line, buffer.c_str());

				strtok(line, ": \n");
				char* tag = strtok(NULL, ": \n");

				if(strstr(buffer.c_str(), "Name : ")!=NULL && tag!=NULL)			name = tag;
				else if(strstr(buffer.c_str(), "Type : ")!=NULL && tag!=NULL)		type = tag;
				else if(strstr(buffer.c_str(), "Texture : ")!=NULL && tag!=NULL)	texture_file_name = tag;
				else if(strstr(buffer.c_str(), "Size : ")!=NULL && tag!=NULL)
				{
					// load from file the Size
					tag=tag+1;
					size.x = atoi(tag=strtok(tag, " ,"));	
					size.y = atoi(tag=strtok(NULL, " )"));
				}
				else if(strstr(buffer.c_str(), "Hitbox : ")!=NULL && tag!=NULL)
				{
					// load from file the Hitbox
					tag=tag+1;
					hitbox.left = atoi(tag=strtok(tag, " ,"));	
					hitbox.top = atoi(tag=strtok(NULL, " ,"));
					hitbox.width = atoi(tag=strtok(NULL, " ,"));	
					hitbox.height = atoi(tag=strtok(NULL, " )"));
				}
			}

			//If an error is met
			if(strcmp(name.c_str(), "")==0 || strcmp(type.c_str(), "")==0 || strcmp(texture_file_name.c_str(), "")==0){ std::cerr << "An error happened while setting the entity characteristics" << std::endl; file_to_load.close(); return -1; }
			else if(strcmp(type.c_str(), "Collector")!=0){ std::cerr << "The entity is not a Collector entity" << std::endl; file_to_load.close(); return -1; }

			// Set Characteristics
			if(size.x!=-1 && size.y!=-1) this->setSize(size);
			if(hitbox.left!=-1 && hitbox.top!=-1 && hitbox.width!=-1 && hitbox.height!=-1) this->setHitbox(hitbox);

			// Texture loading
			if(!texture->loadFromFile(texture_file_name)){ std::cerr << "Error while loading of texture " << texture_file_name << std::endl; return -1; }

			this->getSprite()->setTexture(texture);

		}


		// Animation Tag
		else if(strstr(buffer.c_str(), "[Animation]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Animation]")==NULL)
			{
				char line[LINE_SIZE];
				strcpy(line, buffer.c_str());

				strtok(line, "{\n");
				char* tag = strtok(NULL, "}\n");

				if(strstr(buffer.c_str(), "default : ")!=NULL && tag!=NULL)
				{	
					AnimationParameters anim_prmtrs = animationParametersFromString(std::string(tag));
					if(anim_prmtrs.size.x == -1 || anim_prmtrs.size.y == -1 || anim_prmtrs.init_text_pos.x == -1 || anim_prmtrs.init_text_pos.y == -1 || anim_prmtrs.spacing == -1 || anim_prmtrs.nbr_frames == -1)
						{std::cerr << "An error occured while loading animation parameters" << std::endl; return -1;}
					
					m_animation_parameters.insert( std::pair< std::pair< State, Sense > , AnimationParameters > ( std::pair< State, Sense >(Collector::STANDING, DOWN), anim_prmtrs) );
				}		
			}
		}
		
	}


	file_to_load.close();

	return 0;
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


		
void Collector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw the Collector entity

	if(m_sprite!=NULL)
		target.draw(*m_sprite, states);

};


void Collector::drawPart(sf::RenderWindow* window, unsigned int height)
{
	// Draw a part of the entity designed by the chosen height in parameter
	if(m_sprite!=NULL)
		m_sprite->drawPart(window, height);
}



void Collector::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	if(m_sprite!=NULL)
		m_sprite->drawPartAndAbove(window, height);
}

