#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"


Character::Character()
{
    
}

Character::Character(sf::Texture* texture, sf::IntRect rect_sprite)
{
    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(rect_sprite);
    
    this->setSize(sf::Vector2f(rect_sprite.width, rect_sprite.height));
    this->update();
}

Character::~Character()
{
    
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


//Méthodes

void Character::move(sf::Vector2f movement)
{
    m_position+=movement;
    this->update();
}

void Character::update()
{
    m_sprite.setPosition(m_position);
}

void Character::getDrawn(sf::RenderWindow* window)
{
    window->draw(m_sprite);
}