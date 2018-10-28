#include <SFML/Graphics.hpp>
#include <iostream>
#include "Animation.h"





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

void AnimatedSprite::setInitTxtPos(sf::Vector2f init_text_pos)
{
	// Modifie la position initiale de capture dans la texture
	m_init_txt_pos = init_text_pos;
}

void AnimatedSprite::setParameters(sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, int spacing)
{
	//Modifie les paramètre de capture dans la texture
	m_size = size;
	m_nbr_frames = nbr_frames;
	m_init_txt_pos = init_text_pos;
	m_spacing = spacing;

}

void AnimatedSprite::setPosition(sf::Vector2f position)
{
	// Modifie la position en fonction d'un vecteur pris en paramètres
	m_position=position;
}

void AnimatedSprite::setFPSQuotient(int quotient)
{
	// Modifie le quotient fps
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
	target.draw(m_sprite);
}


