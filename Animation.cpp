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
	return m_texture;
}


sf::Vector2f AnimatedSprite::getSize()
{
	return m_size;
}

sf::Vector2f AnimatedSprite::getPosition()
{
	return m_position;
}

int AnimatedSprite::getSpacing()
{
	return m_spacing;
}


sf::Vector2f AnimatedSprite::getInitTxtPos()
{
	return m_init_txt_pos;
}


sf::Sprite AnimatedSprite::getSprite()
{
	return m_sprite;
}

int AnimatedSprite::getNbrFrames()
{
	return m_nbr_frames;
}


int AnimatedSprite::getActualFrame()
{
	return m_actual_frame;
}





//Modificateurs


void AnimatedSprite::setPosition(sf::Vector2f position)
{
	m_position=position;
}

void AnimatedSprite::setFPSQuotient(int quotient)
{
	m_fps_quotient = quotient;
}


//Méthodes

void AnimatedSprite::update()
{
	m_phase_f_to_f = (m_phase_f_to_f+1)%m_fps_quotient;

	if(m_phase_f_to_f == 0) 
		m_actual_frame = (m_actual_frame+1)%m_nbr_frames;


	m_sprite.setTextureRect( sf::IntRect(m_init_txt_pos.x + m_actual_frame*(m_size.x + m_spacing), m_init_txt_pos.y, m_size.x, m_size.y) );
	m_sprite.setPosition(m_position);
}


void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}


