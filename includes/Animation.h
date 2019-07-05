#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>

#include "Map.h"


typedef struct
{
	sf::Vector2f size;
	int nbr_frames; 
	sf::Vector2f init_text_pos;
	int spacing;

} AnimationParameters;


class AnimatedSprite : public sf::Drawable
{
public:
	AnimatedSprite();
	AnimatedSprite(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos);
	AnimatedSprite(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, int spacing);
	~AnimatedSprite();

	
	// Accesseurs
	sf::Texture* getTexture();
	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	
	int getSpacing();
	sf::Vector2f getInitTxtPos();

	sf::Sprite getSprite();

	int getNbrFrames();
	int getActualFrame();
	
	int getFPSQuotient();
	int getPhaseFToF();

	//Modificateurs;
	void setTexture(sf::Texture* texture);

	void setInitTxtPos(sf::Vector2f init_text_pos);
	void setParameters(sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, int spacing);
	void setPosition(sf::Vector2f position);
	void setFPSQuotient(int quotient);
	
	//Méthodes
	void update();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	sf::Texture* m_texture;
	sf::Vector2f m_size;
	sf::Vector2f m_position;

	int m_spacing;
	sf::Vector2f m_init_txt_pos;

	
	sf::Sprite m_sprite;

	int m_nbr_frames;
	int m_actual_frame;
	
	int m_fps_quotient;	
	int m_phase_f_to_f;
};





class AnimatedSpriteInMap : public AnimatedSprite
{
public:
	AnimatedSpriteInMap();
	AnimatedSpriteInMap(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, float ground_zone_y, Map* map);
	AnimatedSpriteInMap(sf::Texture* texture, sf::Vector2f size, int nbr_frames, sf::Vector2f init_text_pos, int spacing, float ground_zone_y, Map* map);

	int getHeightLevels();

	void update();
	void drawPart(sf::RenderWindow* window, unsigned int height);
	void drawPartAndAbove(sf::RenderWindow* window, unsigned int height);


private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	int 								m_height_levels;

	float						 		m_ground_zone_y;
	Map* 								m_map;

	std::vector<sf::VertexArray> 		m_vertex_arrays;
};






#endif
