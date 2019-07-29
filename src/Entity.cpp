#include "Entity.h"
#include "Animation.h"

#define LINE_SIZE 2000



Entity::Entity()
{

}

/*
Entity::~Entity()
{
	std::cout << "Scurt" << std::endl;
	if(m_sprite != NULL)
		delete m_sprite;
}
*/


// Getters

sf::Texture* Entity::getTexture()
{
	/* Return the pointer to the texture of the entity */
	return m_texture;
}

std::string Entity::getType() const
{
	/* Return the type of the entity */
	return m_type;
}


sf::Vector2f Entity::getPosition() const
{
	/* Return the position of the entity */
	return m_position;
}


sf::Vector2f Entity::getSize() const
{
	/* Return the size of the entity */
	return m_size;
}


int Entity::getHeight() const
{
	/* Return the height of the entity */
	return m_height;
}


int Entity::getGroundZone() const
{
	/* Return the length of the zone parallel with the ground */
	return m_ground_zone;
}


Sense Entity::getSense() const
{
	/* Return sense of the entity */
	return m_sense;
}


AnimatedSprite* Entity::getSprite()
{
	/* Return a pointer to the sprite */
	return m_sprite;
}


bool Entity::isAffiliatedToMap()
{
	return m_affiliated_to_map;
}



// Setters

void Entity::setTexture(sf::Texture* texture)
{
	/* Set the pointer to the texture of the entity */
	m_texture = texture;
}


void Entity::setType(std::string type)
{
	/* Set the type of the entity */
	m_type = type;
}


void Entity::setPosition(sf::Vector2f position)
{
	/* Change the position of the entity */
	m_position = position;
}


void Entity::setHeight(int height)
{
	/* Change the height of the entity */
	m_height=height;
}


void Entity::setSize(sf::Vector2f size)
{	
	/* Change the size of the entity */
	m_size = size;
}


void Entity::setGroundZone(int ground_zone)
{
	/* Change the length of the zone parallel with the ground */
	m_ground_zone = ground_zone;
}


void Entity::setSense(Sense sense)
{
	/* Change the sense of the entity according to the value in parameter */
	m_sense = sense;
}



// Methods


int Entity::loadFromFile(std::string file_name, sf::Texture* texture)
{
	std::ifstream file_to_load;
	file_to_load.open(file_name);
	if(file_to_load.fail()) {std::cerr << "An error happened while opening the entity file" << file_name << std::endl; file_to_load.close(); return -1; }

	std::string name="";
	std::string type="";
	sf::Vector2f size = sf::Vector2f(-1,-1);
	int ground_zone=-1;
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
				else if(strstr(buffer.c_str(), "GroundZone : ")!=NULL && tag!=NULL)	ground_zone = atoi(tag);

			}

			//If an error is met
			if(strcmp(name.c_str(), "")==0 || strcmp(type.c_str(), "")==0 || strcmp(texture_file_name.c_str(), "")==0){ std::cerr << "An error happened while setting the entity characteristics" << std::endl; file_to_load.close(); return -1; }
			else if(strcmp(type.c_str(), this->getType().c_str())!=0){ std::cerr << "The entity is not a " << this->getType() << " entity" << std::endl; file_to_load.close(); return -1; }

			// Set Characteristics
			if(size.x!=-1 && size.y!=-1) this->setSize(size);
			if(ground_zone!=-1) this->setGroundZone(ground_zone);

			// Texture loading
			if(!texture->loadFromFile(texture_file_name)){ std::cerr << "Error while loading of texture " << texture_file_name << std::endl; return -1; }

			this->setTexture(texture);

		}

		// Animation Tag
		else if(strstr(buffer.c_str(), "[Features]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Features]")==NULL)
			{
				readFeaturesFromString(buffer);
			}
		}


		// Animation Tag
		else if(strstr(buffer.c_str(), "[Animations]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Animations]")==NULL)
			{
				readAnimationFromString(buffer)	;
			}
		}
		
	}


	file_to_load.close();

	return 0;
}


void Entity::update()
{
	
}


void Entity::drawPart(sf::RenderWindow* window, unsigned int height)
{
	/* For sub-class, draw a part of sprite corresponding to the height given in parameters */
	(void)height;
	(void)window;
}


void Entity::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	/* For sub-class, draw all the sprite above a height */
	(void)height;
	(void)window;
}


void Entity::readFeaturesFromString(std::string string)
{
	(void)string;
}


void Entity::readAnimationFromString(std::string string)
{
	(void)string;
}