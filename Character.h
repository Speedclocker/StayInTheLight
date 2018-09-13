#include <SFML/Graphics.hpp>
#include <iostream>

class Character
{
public:
    Character();
    Character(sf::Texture* texture, sf::IntRect rect_sprite);
    ~Character();
    
    //Accesseurs 
    int getHealth();
    int getSpeed();

    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    
    
    //Modificateurs 
    void setHealth(int health);
    void setSpeed(int speed);
    
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    
    
    //Méthodes
    void move(sf::Vector2f movement);
    void update();
    
    void getDrawn(sf::RenderWindow* window);
    
private:
    int m_health;
    int m_speed;
    
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    
    sf::Sprite m_sprite;
};