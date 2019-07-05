#include <SFML/Graphics.hpp>
#include "collision.h"
#include "Character.h"
#include "Collector.h"


struct Interval
{
    int a;
    int b;
    int length()
    {
        return b-a;
    }
};



bool collision_rects(sf::IntRect rect_1, sf::IntRect rect_2)
{
	// Renvoie true s'il y a collision entre deux Rect, false sinon.
	if (rect_1.top >= rect_2.top + rect_2.height ||
	    rect_2.top >= rect_1.top + rect_1.height ||
	    rect_1.left >= rect_2.left + rect_2.width ||
	    rect_2.left >= rect_1.left + rect_1.width )
		return false;

	return true;
}



bool collision_characters(Character& character_1, Character& character_2)
{
	// Renvoie true s'il y a collision entre deux Characters, false sinon
	return collision_rects(character_1.getAbsHitbox(), character_2.getAbsHitbox());
}


void physics_characters(Character* character_1, Character* character_2)
{
	// Simule un mouvement physique de poussée entre deux Characters
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
				character_2->move(sf::Vector2f(0,dec_y-dec_y/2));
			}
			else
			{
				character_1->move(sf::Vector2f(0,dec_y-dec_y/2));
				character_2->move(sf::Vector2f(0,-dec_y/2));
			}
		}
		else
		{
			if(character_1->getAbsHitbox().left + character_1->getAbsHitbox().width/2 < character_2->getAbsHitbox().left + character_2->getAbsHitbox().width/2)
			{
				character_1->move(sf::Vector2f(-dec_x/2,0));
				character_2->move(sf::Vector2f(dec_x-dec_x/2,0));
			}
			else
			{
				character_1->move(sf::Vector2f(dec_x-dec_x/2,0));
				character_2->move(sf::Vector2f(-dec_x/2,0));
			}
		}
	}
}


void physics_character_collector(Character* character, Collector* collector)
{
    if(collision_rects(character->getAbsHitbox(), collector->getAbsHitbox()))
    {
        int dec_x=0, dec_y=0;

        if(character->getAbsHitbox().left + character->getAbsHitbox().width/2 < collector->getAbsHitbox().left + collector->getAbsHitbox().width/2)
            dec_x = ((character->getAbsHitbox().left + character->getAbsHitbox().width) - collector->getAbsHitbox().left);
        else
            dec_x = ((collector->getAbsHitbox().left + collector->getAbsHitbox().width) - character->getAbsHitbox().left);


        if(character->getAbsHitbox().top + character->getAbsHitbox().height/2 < collector->getAbsHitbox().top + collector->getAbsHitbox().height/2)
            dec_y = ((character->getAbsHitbox().top + character->getAbsHitbox().height) - collector->getAbsHitbox().top);
        else
            dec_y = ((collector->getAbsHitbox().top + collector->getAbsHitbox().height) - character->getAbsHitbox().top);



        if(dec_x > dec_y)
        {
            if(character->getAbsHitbox().top + character->getAbsHitbox().height/2 < collector->getAbsHitbox().top + collector->getAbsHitbox().height/2)
                character->move(sf::Vector2f(0,-dec_y));
            else
                character->move(sf::Vector2f(0,dec_y));
        }
        else
        {
            if(character->getAbsHitbox().left + character->getAbsHitbox().width/2 < collector->getAbsHitbox().left + collector->getAbsHitbox().width/2)
                character->move(sf::Vector2f(-dec_x,0));
            else
                character->move(sf::Vector2f(dec_x,0));
        }
    }
}


void physics_character_map(Character* character, Map* map, int height)
{
    struct Interval temp_vertical1{-1,-1};
    struct Interval temp_vertical2{-1,-1};
    struct Interval temp_horizontal1{-1,-1};
    struct Interval temp_horizontal2{-1,-1};


    std::vector < sf::IntRect > temp_hitbox_array;

    for(int i=character->getAbsHitbox().left; i <= character->getAbsHitbox().left + character->getAbsHitbox().width + map->getTileSize(); i+=map->getTileSize())
    {
		for(int j=character->getAbsHitbox().top; j <= character->getAbsHitbox().top + character->getAbsHitbox().height+ map->getTileSize(); j+=map->getTileSize())
        {
            if(i>=0 && i<map->getSize().x*map->getTileSize() && j>=0 && j<map->getSize().y*map->getTileSize() && map->getTileFromCoords(height, sf::Vector2f(i, j)).m_collisionable )
            {
            	int tile_sz = map->getTileSize();
                sf::IntRect temp_hitbox_rect( sf::Vector2i( (int)(i/tile_sz)*tile_sz , (int)(j/tile_sz)*tile_sz ), sf::Vector2i(tile_sz, tile_sz) );

                temp_hitbox_array.push_back(temp_hitbox_rect);
            }
        }
    }

    for(std::vector< sf::IntRect >::iterator hit1 = temp_hitbox_array.begin() ; hit1 != temp_hitbox_array.end(); hit1++)
    {
        if( collision_rects(character->getAbsHitbox(), *hit1) )
        {
            if(hit1->top+hit1->height/2<character->getAbsHitbox().top+character->getAbsHitbox().height/2)
            {
                temp_vertical1.a=character->getAbsHitbox().top;
                temp_vertical2.a=character->getAbsHitbox().top;

                temp_vertical1.b=hit1->top+hit1->height;
                temp_vertical2.b=hit1->top+hit1->height;
            }
            else
            {
                temp_vertical1.a=character->getAbsHitbox().top+character->getAbsHitbox().height;
                temp_vertical2.a=character->getAbsHitbox().top+character->getAbsHitbox().height;

                temp_vertical1.b=hit1->top;
                temp_vertical2.b=hit1->top;
            }

            if(hit1->left+hit1->width/2<character->getAbsHitbox().left+character->getAbsHitbox().width/2)
            {
                temp_horizontal1.a=character->getAbsHitbox().left;
                temp_horizontal2.a=character->getAbsHitbox().left;

                temp_horizontal1.b=hit1->left+hit1->width;
                temp_horizontal2.b=hit1->left+hit1->width;
            }
            else
             {
                temp_horizontal1.a=character->getAbsHitbox().left+character->getAbsHitbox().width;
                temp_horizontal2.a=character->getAbsHitbox().left+character->getAbsHitbox().width;

                temp_horizontal1.b=hit1->left;
                temp_horizontal2.b=hit1->left;
            }


            for(std::vector< sf::IntRect >::iterator hit2 = temp_hitbox_array.begin() ; hit2 != temp_hitbox_array.end(); hit2++)
    		{
                if(collision_rects(character->getAbsHitbox(), (*hit2)) && hit1!=hit2)
                {
                    if(hit1->top+hit1->height/2<character->getAbsHitbox().top+character->getAbsHitbox().height/2
                   	&& hit2->top+hit2->height/2<character->getAbsHitbox().top+character->getAbsHitbox().height/2)
                    {
                        if(temp_vertical1.b>hit2->top+hit2->height)		temp_vertical1.b=hit2->top+hit2->height;


                        if(hit1->left+hit1->width/2<hit2->left+hit2->width/2)
                        {
                            if(hit2->left+hit2->width>character->getAbsHitbox().left+character->getAbsHitbox().width && temp_horizontal1.b<character->getAbsHitbox().left+character->getAbsHitbox().width)
                                temp_horizontal1.b=character->getAbsHitbox().left+character->getAbsHitbox().width;

                            else if(hit2->left+hit2->width<=character->getAbsHitbox().left+character->getAbsHitbox().width && temp_horizontal1.b<hit2->left+hit2->width)
                               	temp_horizontal1.b=hit2->left+hit2->width;

                        }
                        else
                        {
                            if(hit2->left<character->getAbsHitbox().left && temp_horizontal1.a>character->getAbsHitbox().left)	
                            	temp_horizontal1.a=character->getAbsHitbox().left;

                            else if(hit2->left>=character->getAbsHitbox().left && temp_horizontal1.a>hit2->left)				
                            	temp_horizontal1.a=hit2->left;
                        }
                    }

                    else if(hit1->top+hit1->height/2>=character->getAbsHitbox().top+character->getAbsHitbox().height/2
                         && hit2->top+hit2->height/2>=character->getAbsHitbox().top+character->getAbsHitbox().height/2)
                    {
                        if(temp_vertical1.b<hit2->top) 	temp_vertical1.b=hit2->top;

                        if(hit1->left+hit1->width/2<hit2->left+hit2->width/2)
                        {
                            if(hit2->left+hit2->width>character->getAbsHitbox().left+character->getAbsHitbox().width
                               && temp_horizontal1.b<character->getAbsHitbox().left+character->getAbsHitbox().width)
                                temp_horizontal1.b=character->getAbsHitbox().left+character->getAbsHitbox().width;

                            else if(hit2->left+hit2->width<=character->getAbsHitbox().left+character->getAbsHitbox().width
                               && temp_horizontal1.b<hit2->left+hit2->width)
                               	temp_horizontal1.b=hit2->left+hit2->width;

                        }
                        else
                        {
                            if(hit2->left<character->getAbsHitbox().left && temp_horizontal1.a>character->getAbsHitbox().left) 
                            	temp_horizontal1.a=character->getAbsHitbox().left;

                            else if(hit2->left>=character->getAbsHitbox().left && temp_horizontal1.a>hit2->left)
                               	temp_horizontal1.a=hit2->left;
                        }
                    }

                    if(hit1->left+hit1->width/2<character->getAbsHitbox().left+character->getAbsHitbox().width/2 
                    && hit2->left+hit2->width/2<character->getAbsHitbox().left+character->getAbsHitbox().width/2)
                    {
                        if(temp_horizontal2.b>hit2->left+hit2->width) 	temp_horizontal2.b=hit2->left+hit2->width;


                        if(hit1->top+hit1->height/2<hit2->top+hit2->height/2)
                        {
                            if(hit2->top+hit2->height>character->getAbsHitbox().top+character->getAbsHitbox().height
                               && temp_vertical2.b<character->getAbsHitbox().top+character->getAbsHitbox().height)
                                temp_vertical2.b=character->getAbsHitbox().top+character->getAbsHitbox().height;

                            else if(hit2->top+hit2->height<=character->getAbsHitbox().top+character->getAbsHitbox().height
                               && temp_vertical2.b<hit2->top+hit2->height)
                            	temp_vertical2.b=hit2->top+hit2->height;

                        }
                        else
                        {
                            if(hit2->top<character->getAbsHitbox().top && temp_vertical2.a>character->getAbsHitbox().top)
                                temp_vertical2.a=character->getAbsHitbox().top;

                            else if(hit2->top>=character->getAbsHitbox().top && temp_vertical2.a>hit2->top)
                               	temp_vertical2.a=hit2->top;
                        }
                    }

                    else if(hit1->left+hit1->width/2>=character->getAbsHitbox().left+character->getAbsHitbox().width/2
                       && hit2->left+hit2->width/2>=character->getAbsHitbox().left+character->getAbsHitbox().width/2)
                    {
                        if(temp_horizontal2.b<hit2->left) temp_horizontal2.b=hit2->left;

                        if(hit1->top+hit1->height/2<hit2->top+hit2->height/2)
                        {
                            if(hit2->top+hit2->height>character->getAbsHitbox().top+character->getAbsHitbox().height
                               && temp_vertical2.b<character->getAbsHitbox().top+character->getAbsHitbox().height)
                                temp_vertical2.b=character->getAbsHitbox().top+character->getAbsHitbox().height;

                            else if(hit2->top+hit2->height<=character->getAbsHitbox().top+character->getAbsHitbox().height
                               && temp_vertical2.b<hit2->top+hit2->height)
                               	temp_vertical2.b=hit2->top+hit2->height;

                        }
                        else
                        {
                            if(hit2->top<character->getAbsHitbox().top && temp_vertical2.a>character->getAbsHitbox().top)
                                temp_vertical2.a=character->getAbsHitbox().top;

                            else if(hit2->top>=character->getAbsHitbox().top && temp_vertical2.a>hit2->top)
                               	temp_vertical2.a=hit2->top;
                        }
                    }

                }
            }


            if(temp_horizontal2.length()==temp_horizontal1.length() && temp_vertical2.length()==temp_vertical1.length())
            {
                if(abs(temp_horizontal1.length())>abs(temp_vertical1.length()))
                    character->move(0, temp_vertical1.length());
                else if(abs(temp_horizontal1.length())<=abs(temp_vertical1.length()))
                    character->move(temp_horizontal1.length(),0);
            }
            else if (temp_horizontal2.length()==temp_horizontal1.length())
            {
                if(abs(temp_vertical1.length())>abs(temp_vertical2.length()))
                {
                    if(abs(temp_horizontal1.length())>abs(temp_vertical1.length()))
                        character->move(0, temp_vertical1.length());
                    else if(abs(temp_horizontal1.length())<=abs(temp_vertical1.length()))
                        character->move(temp_horizontal1.length(),0);
                }
                else if(abs(temp_vertical1.length())<=abs(temp_vertical2.length()))
                {
                    if(abs(temp_horizontal1.length())>abs(temp_vertical2.length()))
                        character->move(0, temp_vertical2.length());
                    else if(abs(temp_horizontal1.length())<=abs(temp_vertical2.length()))
                        character->move(temp_horizontal1.length(),0);
                }
            }
            else if (temp_vertical2.length()==temp_vertical1.length())
            {
                if(abs(temp_horizontal1.length())>abs(temp_horizontal2.length()))
                {
                    if(abs(temp_horizontal1.length())>abs(temp_vertical1.length()))
                        character->move(0, temp_vertical1.length());
                    else if(abs(temp_horizontal1.length())<=abs(temp_vertical1.length()))
                        character->move(temp_horizontal1.length(),0);
                }
                else if(abs(temp_horizontal1.length())<=abs(temp_horizontal2.length()))
                {
                    if(abs(temp_horizontal2.length())>abs(temp_vertical1.length()))
                        character->move(0, temp_vertical1.length());
                    else if(abs(temp_horizontal2.length())<=abs(temp_vertical1.length()))
                        character->move(temp_horizontal1.length(),0);
                }
            }
            else
            {
                if(abs(temp_horizontal2.length())>abs(temp_vertical2.length()) && abs(temp_horizontal1.length())>abs(temp_vertical1.length()))
                {
                        character->move(0, temp_vertical2.length());
                        character->move(0, temp_vertical1.length());
                }
                else if(abs(temp_horizontal2.length())<=abs(temp_vertical2.length()) && abs(temp_horizontal1.length())>abs(temp_vertical1.length()))
                {
                        character->move(temp_horizontal2.length(), 0);
                        character->move(0, temp_vertical1.length());
                }
                else if(abs(temp_horizontal2.length())>abs(temp_vertical2.length()) && abs(temp_horizontal1.length())<=abs(temp_vertical1.length()))
                {
                        character->move(0, temp_vertical2.length());
                        character->move(temp_horizontal1.length(), 0);
                }
                else if(abs(temp_horizontal2.length())<=abs(temp_vertical2.length()) && abs(temp_horizontal1.length())<=abs(temp_vertical1.length()))
                {
                        character->move(temp_horizontal2.length(), 0);
                        character->move(temp_horizontal1.length(), 0);
                }
            }
        }
    }
}
