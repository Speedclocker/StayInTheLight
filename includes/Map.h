#ifndef __MAP_H__
#define __MAP_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Object.h"

typedef struct
{
	sf::Vector2f	m_pos_text;
	sf::Vector2f	m_size_text;
	bool 			m_collisionable;
} Tile ;

class Map : public sf::Drawable, public sf::Transformable
{
public:
	Map(void);
	Map(sf::Vector2f size, int height, int tile_sz);
	~Map();

	int getHeight() const;
	sf::Vector2f getSize() const;
	int getTileSize() const;
	Tile getTile(int height, sf::Vector2i tile_pos) const;


	void setTile(Tile tile, sf::Vector2i position, int height);
	void setTexture(sf::Texture* texture);


	void update();
	void update_transparency(int chosen_height);

	

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Tile*** 			m_map;
	Object*				m_objects;
	sf::VertexArray 	m_vertex;
	sf::Texture* 		m_texture;
	int 				m_height;
	sf::Vector2f 		m_size;
	int					m_tile_sz;
};


#endif