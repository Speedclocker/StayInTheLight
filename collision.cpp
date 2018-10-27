#include <SFML/Graphics.hpp>
#include "collision.h"
#include "Character.h"



bool collision_rects(sf::IntRect rect_1, sf::IntRect rect_2)
{
    if (rect_1.top > rect_2.top + rect_2.height ||
        rect_2.top > rect_1.top + rect_1.height ||
        rect_1.left > rect_2.left + rect_2.width ||
        rect_2.left > rect_1.left + rect_1.width )
        
        return false;
    
    return true;
}



bool collision_characters(Character character_1, Character character_2)
{
    return collision_rects(sf::IntRect(character_1.getHitbox().left + character_1.getPosition().x, 
                                       character_1.getHitbox().top + character_1.getPosition().y,
                                       character_1.getHitbox().width, character_1.getHitbox().height),
                           sf::IntRect(character_2.getHitbox().left + character_2.getPosition().x, 
                                       character_2.getHitbox().top + character_2.getPosition().y,
                                       character_2.getHitbox().width, character_2.getHitbox().height));
}


void physics_characters(Character* character_1, Character* character_2)
{
    if(collision_characters(*character_1, *character_2))
    {
        int dec_x=0, dec_y=0;
        
        if(character_1->getAbsHitbox().left + character_1->getAbsHitbox().width/2 < character_2->getAbsHitbox().left + character_2->getAbsHitbox().width/2)
            dec_x = ((character_1->getAbsHitbox().left + character_1->getAbsHitbox().width) - character_2->getAbsHitbox().left);
        else
            dec_x = ((character_2->getAbsHitbox().left + character_2->getAbsHitbox().width) - character_1->getAbsHitbox().left);
        
        
        if(character_1->getAbsHitbox().top + character_1->getAbsHitbox().height/2 < character_2->getAbsHitbox().top + character_2->getAbsHitbox().height/2)
            dec_y = ((character_1->getAbsHitbox().top + character_1->getAbsHitbox().height) - character_2->getAbsHitbox().top);
        else
            dec_y = ((character_2->getAbsHitbox().top + character_2->getAbsHitbox().height) - character_1->getAbsHitbox().top);
        
        
        if(dec_x > dec_y)
        {
            if(character_1->getAbsHitbox().top + character_1->getAbsHitbox().height/2 < character_2->getAbsHitbox().top + character_2->getAbsHitbox().height/2)
            {
                character_1->move(sf::Vector2f(0,-dec_y/2));
                character_2->move(sf::Vector2f(0, dec_y/2));
            }
            else
            {
                character_1->move(sf::Vector2f(0, dec_y/2));
                character_2->move(sf::Vector2f(0,-dec_y/2));
            }
        }
        else
        {
            if(character_1->getAbsHitbox().left + character_1->getAbsHitbox().width/2 < character_2->getAbsHitbox().left + character_2->getAbsHitbox().width/2)
            {
                character_1->move(sf::Vector2f(-dec_x,0));
                character_2->move(sf::Vector2f( dec_x,0));
            }
            else
            {
                character_1->move(sf::Vector2f(dec_x,0));
                character_2->move(sf::Vector2f(-dec_x,0));
            }
        }
            
        
    }
}