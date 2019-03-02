#include "Map.h"

#define TILE_SZ 20

Map::Map(void)
{
	
}



Map::Map(sf::Vector2f size, int height)
{
	m_height=height;
	m_size=size;
	

	//Allocation espace map

	m_map = (Tile***)malloc( m_height * sizeof(Tile**));
	for(int h=0; h<height; h++)
	{
		m_map[h] = (Tile**)malloc( m_size.x * sizeof(Tile*) );
		for(int i=0; i<m_size.x; i++)
		{
			m_map[h][i] = (Tile*)malloc(m_size.y * sizeof(Tile));
			for(int j=0; j<m_size.y; j++)
			{
				m_map[h][i][j].m_collisionable=true;
			}
		}
	}	
}
	


//Taille de la map
int Map::getHeight() const		{	return m_height;	}


//Hauteur de la map
sf::Vector2f Map::getSize() const	{	return m_size;		}


void Map::update()
{
	m_vertex.setPrimitiveType(sf::Quads);
	m_vertex.resize(this->getSize().x * this->getSize().y * this->getHeight() * 4);

	int index=0;	
	for(int h=0; h<this->getHeight(); h++)
	{
		for(int i=0; i<this->getSize().x; i++)
		{
			for(int j=0; j<this->getSize().y; j++)
			{
				index = h*this->getSize().x * this->getSize().y * 4 + i*this->getSize().y * 4 + j*4;

				
			}
		}	
	}
}


		
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(int h=0; h < this->getHeight(); h++)
	{
		for(int i=0; i < this->getSize().x; i++)
		{
			for(int j=0; j<this->getSize().y; j++)
			{
				m_map[h][i][j];
			}
		}
	}
	//target.draw();
};


