#include "Map.h"


Map::Map(void)
{
	
}



Map::Map(sf::Vector2f size, int height, int tile_sz)
{
	m_height=height;
	m_size=size;
	m_tile_sz=tile_sz;
	

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
				m_map[h][i][j].m_pos_text=sf::Vector2f(0,0);

				m_map[h][i][j].m_collisionable=true;
			}
		}
	}	
}
	
Map::~Map()
{

}


//Taille de la map
int Map::getHeight() const		{	return m_height;	}


//Hauteur de la map
sf::Vector2f Map::getSize() const	{	return m_size;		}


//Taille d'une tile
int Map::getTileSize() const		{	return m_tile_sz;	}



void Map::setTile(Tile tile, sf::Vector2i position, int height)
{
	m_map[height][position.x][position.y]=tile;
}



void Map::setTexture(sf::Texture* texture)
{
	m_texture=texture;
}



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

				m_vertex[index].position = sf::Vector2f(i*m_tile_sz, j*m_tile_sz);
				m_vertex[index+1].position = sf::Vector2f((i+1)*m_tile_sz, j*m_tile_sz);
				m_vertex[index+2].position = sf::Vector2f((i+1)*m_tile_sz, (j+1)*m_tile_sz);
				m_vertex[index+3].position = sf::Vector2f(i*m_tile_sz, (j+1)*m_tile_sz);

				m_vertex[index].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(0, 0);
				m_vertex[index+1].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, 0);
				m_vertex[index+2].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, m_tile_sz);
				m_vertex[index+3].texCoords= m_map[h][i][j].m_pos_text + sf::Vector2f(0, m_tile_sz);


				m_vertex[index].color.a = 255;
				m_vertex[index+1].color.a = 255;
				m_vertex[index+2].color.a = 255;
				m_vertex[index+3].color.a = 255;
				

				
			}
		}	
	}
}


void Map::update_transparency(int chosen_height)
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

				m_vertex[index].position = sf::Vector2f(i*m_tile_sz, j*m_tile_sz);
				m_vertex[index+1].position = sf::Vector2f((i+1)*m_tile_sz, j*m_tile_sz);
				m_vertex[index+2].position = sf::Vector2f((i+1)*m_tile_sz, (j+1)*m_tile_sz);
				m_vertex[index+3].position = sf::Vector2f(i*m_tile_sz, (j+1)*m_tile_sz);

				m_vertex[index].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(0, 0);
				m_vertex[index+1].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, 0);
				m_vertex[index+2].texCoords = m_map[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, m_tile_sz);
				m_vertex[index+3].texCoords= m_map[h][i][j].m_pos_text + sf::Vector2f(0, m_tile_sz);

				if((int)(index/(this->getSize().x * this->getSize().y * 4)) != chosen_height)
				{
					m_vertex[index].color.a = 50;
					m_vertex[index+1].color.a = 50;
					m_vertex[index+2].color.a = 50;
					m_vertex[index+3].color.a = 50;
				}
				else
				{
					m_vertex[index].color.a = 255;
					m_vertex[index+1].color.a = 255;
					m_vertex[index+2].color.a = 255;
					m_vertex[index+3].color.a = 255;
				}

			}
		}	
	}
}

		
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	states.texture = m_texture;

	target.draw(m_vertex, states);
};


