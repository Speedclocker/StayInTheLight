#ifndef __MAP_H__
#define __MAP_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "Camera.h"

typedef struct
{
	sf::Vector2f			m_pos_text;
	sf::Vector2f			m_size_text;
	bool 					m_collisionable;
} Tile ;


class Map : public sf::Drawable, public sf::Transformable
{
public:
	// Constructors/Destructors
	Map();
	Map(sf::Vector2f size, int height, int tile_sz);
	~Map();


	// Getters
	int getHeight() const;
	sf::Vector2f getSize() const;
	int getTileSize() const;
	Tile getTile(int height, sf::Vector2i tile_pos) const;
	Tile getTileFromCoords(int height, sf::Vector2f position) const;


	// Setters
	void setSize(unsigned int size_x, unsigned int size_y);
	void setHeight(unsigned int height);
	void setTile(Tile tile, sf::Vector2i position, int height);
	void setTexture(sf::Texture* texture);
	void setCamera(Camera* Camera);


	//Methods
	void update();
	void update_transparency(int chosen_height);
	void physics_entities();
	void addEntity(Entity* entity);
	void delEntity(Entity* entity);
	

private:
	void updateEntities();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	clock_t																				m_time;			
	Tile*** 																			m_tiles;			// Tiles of the map
	std::vector< std::vector<Entity*> >													m_entities_to_draw;
	std::vector< std::vector< std::vector< std::vector <CollisionableEntity*> > > >		m_entities_coll;	// All entities in map
	std::vector<Entity*>																m_entities;			// All entities in map
	sf::VertexArray 																	m_vertex;			// Vertex used to draw the map tiles
	sf::Texture* 																		m_texture;			// Texture associated to the map
	int 																				m_height;			// Number of levels of height
	sf::Vector2f 																		m_size;				// Size of the map in number of tiles
	int																					m_tile_sz;			// Size of a tile in pixel
	Camera* 																			m_camera; 			// Camera on map (if there is one, else NULL)
};


bool comparePosY(Entity* ent1, Entity* ent2);


#endif