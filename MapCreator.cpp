#include <SFML/Graphics.hpp>
#include <iostream>
#include "MapCreator.h"

struct Tile
{
	sf::Texture*	m_texture;
	sf::Vector2f	m_pos_text;
	sf::Vector2f	m_size_text;
	bool 		m_collisionable;
};

class Map
{
public:
	Map();
	Map(sf::Vector2f size, int height);
	
		
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	

	Tile*** map;
	
};

int main()
{
	
	return 0;
}
