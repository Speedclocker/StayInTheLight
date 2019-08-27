#include <SFML/Graphics.hpp>
#include <iostream>
#include "Animation.h"

#define STR_SIZE 300



AnimationParameters animationParametersFromString(std::string string)
{
	/* Set Animation parameters from string. String format must be : (size.x,size.y),nbr_frames,(init_text_pos.x,init_text_pos.y),spacing */

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



bool operator==(AnimationParameters const& a, AnimationParameters const& b) { return (a.size == b.size && a.nbr_frames == b.nbr_frames && a.init_text_pos == b.init_text_pos && a.spacing == b.spacing); }
bool operator!=(AnimationParameters const& a, AnimationParameters const& b) { return !(a.size == b.size && a.nbr_frames == b.nbr_frames && a.init_text_pos == b.init_text_pos && a.spacing == b.spacing); }




//---------------------------------------------------- Animation -----------------------------------------------//

AnimatedSprite::AnimatedSprite()
{

}


AnimatedSprite::AnimatedSprite(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos)
{
	m_texture = texture;
	m_size = size;
	m_actual_frame = 0;
	m_nbr_frames = nbr_frames;
	m_init_txt_pos = init_text_pos;
	m_spacing = 0;

	m_fps_quotient = 1;
	m_phase_f_to_f = 0;

	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect( sf::IntRect(m_init_txt_pos.x, m_init_txt_pos.y, m_size.x, m_size.y) );
}


AnimatedSprite::AnimatedSprite(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, int spacing)
{
	m_texture = texture;
	m_size = size;
	m_actual_frame = 0;
	m_nbr_frames = nbr_frames;
	m_init_txt_pos = init_text_pos;
	m_spacing = spacing;

	m_fps_quotient = 1;
	m_phase_f_to_f = 0;

	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect( sf::IntRect(m_init_txt_pos.x, m_init_txt_pos.y, m_size.x, m_size.y) );
}


AnimatedSprite::~AnimatedSprite()
{

}


sf::Texture* AnimatedSprite::getTexture()
{
	// Renvoie un pointeur vers la texture utilisée
	return m_texture;
}


sf::Vector2f AnimatedSprite::getSize()
{
	// Renvoie la taille du sprite animé
	return m_size;
}

sf::Vector2f AnimatedSprite::getPosition()
{
	// Renvoie la position du sprite animé
	return m_position;
}

int AnimatedSprite::getSpacing()
{
	// Renvoie le décalage de frames (espace séparant deux frames sur une même ligne dans la texture)
	return m_spacing;
}


sf::Vector2f AnimatedSprite::getInitTxtPos()
{	
	// Renvoie le position initiale de capture dans la texture
	return m_init_txt_pos;
}


sf::Sprite AnimatedSprite::getSprite()
{
	// Renvoie le sprite
	return m_sprite;
}

int AnimatedSprite::getNbrFrames()
{
	// Renvoie le nombre de frames de l'animation
	return m_nbr_frames;
}


int AnimatedSprite::getActualFrame()
{
	// Renvoie la frame actuelle de l'animation
	return m_actual_frame;
}

int AnimatedSprite::getFPSQuotient()
{
	// Renvoie le quotient fps (le nombre de fps de l'animation sera divisé par ce quotient)
	return m_fps_quotient;
}

int AnimatedSprite::getPhaseFToF()
{
	// Renvoie l'état transitionelle entre deux frames 
	// (utilisé avec le fps quotient, s'incrémente à chaque mise à jour jusqu'à atteindre le fps quotient pour changer de frame)
	return m_phase_f_to_f;
}

//Modificateurs

void AnimatedSprite::setTexture(sf::Texture* texture)
{
	m_texture = texture;
}

void AnimatedSprite::setInitTxtPos(sf::Vector2f init_text_pos)
{
	// Modifie la position initiale de capture dans la texture
	m_init_txt_pos = init_text_pos;
}

void AnimatedSprite::setParameters(AnimationParameters parameters)
{
	//Modifie les paramètres de capture dans la texture
	if(parameters != AnimationParameters{m_size, m_nbr_frames, m_init_txt_pos, m_spacing})
	{
		m_size = parameters.size;
		m_nbr_frames = parameters.nbr_frames;
		m_init_txt_pos = parameters.init_text_pos;
		m_spacing = parameters.spacing;
		m_actual_frame = 0;
	}
}



void AnimatedSprite::setPosition(sf::Vector2f position)
{
	// Modifie la position en fonction d'un vecteur pris en paramètres
	m_position=position;
}

void AnimatedSprite::setFPSQuotient(int quotient)
{
	// Modifie le quotient fps (sert à réduire la vitesse)
	m_fps_quotient = quotient;
}


//Méthodes

void AnimatedSprite::update()
{
	// Met à jour en incrémentant la phase frame à frame, ou en incrémentant la frame actuelle
	m_phase_f_to_f = (m_phase_f_to_f+1)%m_fps_quotient;

	if(m_phase_f_to_f == 0) 
		m_actual_frame = (m_actual_frame+1)%m_nbr_frames;

	m_sprite.setTextureRect( sf::IntRect(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), m_init_txt_pos.y, m_size.x, m_size.y) );
	m_sprite.setPosition(m_position);
}


void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Dessine le sprite

	target.draw(m_sprite, states);
}








AnimatedSpriteInMap::AnimatedSpriteInMap() : AnimatedSprite()
{

}


AnimatedSpriteInMap::AnimatedSpriteInMap(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, float ground_zone_y, Map* map) :
AnimatedSprite(texture, size, nbr_frames, init_text_pos)
{
	m_ground_zone_y = ground_zone_y;
	m_map = map;

	m_height_levels = 0;
}




AnimatedSpriteInMap::AnimatedSpriteInMap(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, int spacing, float ground_zone_y, Map* map) :
AnimatedSprite(texture, size, nbr_frames, init_text_pos, spacing)
{
	ground_zone_y = ground_zone_y;
	m_map = map;

	m_height_levels = 0;

}



int AnimatedSpriteInMap::getHeightLevels()
{
	// Return thh height levels for which the sprite is distributed
	return m_height_levels;
}




void AnimatedSpriteInMap::update()
{
	// Met à jour en incrémentant la phase frame à frame, ou en incrémentant la frame actuelle
	m_phase_f_to_f = (m_phase_f_to_f+1)%m_fps_quotient;

	if(m_phase_f_to_f == 0) 
		m_actual_frame = (m_actual_frame+1)%m_nbr_frames;


	m_sprite.setTextureRect( sf::IntRect(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), m_init_txt_pos.y, m_size.x, m_size.y) );
	m_sprite.setPosition(m_position);

	m_vertex_arrays.clear();

	m_vertex_arrays.push_back(sf::VertexArray(sf::Quads));
	m_vertex_arrays[0].resize(4);
	m_vertex_arrays[0].setPrimitiveType(sf::Quads);


	int tmp_pos = (int)(( m_position.y + m_size.y - m_ground_zone_y) / m_map->getTileSize() ) * m_map->getTileSize(); // The first level above the ground zone
	if(tmp_pos < m_position.y) tmp_pos = m_position.y;
	

	
	m_vertex_arrays[0][0].position = sf::Vector2f(m_position.x, tmp_pos);
	m_vertex_arrays[0][1].position = sf::Vector2f(m_position.x + m_size.x, tmp_pos);
	m_vertex_arrays[0][2].position = m_position + sf::Vector2f(m_size.x, m_size.y);
	m_vertex_arrays[0][3].position = m_position + sf::Vector2f(0, m_size.y);

	m_vertex_arrays[0][0].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), 				m_init_txt_pos.y + (tmp_pos - m_position.y) );
	m_vertex_arrays[0][1].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing) + m_size.x, 	m_init_txt_pos.y + (tmp_pos - m_position.y) );
	m_vertex_arrays[0][2].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing) + m_size.x, 	m_init_txt_pos.y + m_size.y);
	m_vertex_arrays[0][3].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), 				m_init_txt_pos.y + m_size.y);

	int i=0;
	while(m_vertex_arrays[i][0].texCoords.y - m_init_txt_pos.y > 0)
	{
		if(m_vertex_arrays[i][0].texCoords.y - m_map->getTileSize() > m_init_txt_pos.y)
			tmp_pos = m_vertex_arrays[i][0].texCoords.y - m_map->getTileSize(); 
		else
			tmp_pos = m_init_txt_pos.y; 

		m_vertex_arrays.push_back(sf::VertexArray(sf::Quads));
		m_vertex_arrays[i+1].resize(4);
		m_vertex_arrays[i+1].setPrimitiveType(sf::Quads);


		m_vertex_arrays[i+1][0].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), tmp_pos);
		m_vertex_arrays[i+1][1].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing) + m_size.x, tmp_pos);
		m_vertex_arrays[i+1][2].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing) + m_size.x, m_vertex_arrays[i][1].texCoords.y);
		m_vertex_arrays[i+1][3].texCoords = sf::Vector2f(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), m_vertex_arrays[i][0].texCoords.y);

		
		i++;
	}

	for(std::vector<sf::VertexArray>::iterator it = m_vertex_arrays.begin(); it != m_vertex_arrays.end(); it++)
	{
		for(unsigned int vx = 0; vx < it->getVertexCount(); vx++)
			(*it)[vx].position = m_position + sf::Vector2f((*it)[vx].texCoords.x - (m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing)), (*it)[vx].texCoords.y - m_init_txt_pos.y);
	}

	m_height_levels = m_vertex_arrays.size();
}



void AnimatedSpriteInMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

	states.texture = m_texture;

	target.draw(m_sprite, states);
}

void AnimatedSpriteInMap::drawPart(sf::RenderWindow* window, unsigned int height)
{	
	if(height < m_vertex_arrays.size())
		window->draw(m_vertex_arrays[height], m_texture);
}

void AnimatedSpriteInMap::drawPartAndAbove(sf::RenderWindow* window, unsigned int height)
{
	for(int i=height; i<this->getHeightLevels(); i++)
		window->draw(m_vertex_arrays[i], m_texture);
}