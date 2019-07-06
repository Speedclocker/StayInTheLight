#include "Map.h"
#include "collision.h"
#include "Character.h"
#include "Collector.h"


Map::Map()
{
	/* Constructor */
}



Map::Map(sf::Vector2f size, int height, int tile_sz)
{
	/* Constructor */

	m_height 	= height;
	m_size		= size;
	m_tile_sz	= tile_sz;
	
	//Allocation memory for the map
	m_tiles 	= (Tile***)malloc( m_height * sizeof(Tile**));
	for(int h=0; h<m_height; h++)
	{
		m_tiles[h] = (Tile**)malloc( (int)m_size.x * sizeof(Tile*) );
		for(int i=0; i<m_size.x; i++)
		{
			m_tiles[h][i] = (Tile*)malloc( (int)m_size.y * sizeof(Tile));
			for(int j=0; j<m_size.y; j++)
			{
				m_tiles[h][i][j].m_pos_text=sf::Vector2f(0,0);
				m_tiles[h][i][j].m_collisionable=false;
			}
		}
	}	

	this->update();
}


	
Map::~Map()
{
	/* Destructor */
	for(int h=0; h<this->getHeight(); h++)
	{
		for(int i=0; i<this->getSize().x; i++)
		{
			if(m_tiles[h][i]!=NULL)
				free(m_tiles[h][i]);
		}

		if(m_tiles[h]!=NULL)
			free(m_tiles[h]);
	}	
	free(m_tiles);	
}




// Getters

int Map::getHeight() const
{	
	/* Return the map size */
	return m_height;	
}


sf::Vector2f Map::getSize() const
{
	/* Return the map height */
	return m_size;		
}


int Map::getTileSize() const
{	
	/* Return the tile size	*/
	return m_tile_sz;	
}


Tile Map::getTile(int height, sf::Vector2i tile_pos) const 		
{	
	/* Return a specific tile of the map with the indexes given in parameters */
	
	if(m_tiles!=NULL && m_tiles[height]!=NULL && m_tiles[height][tile_pos.x]!=NULL) 
		return m_tiles[height][tile_pos.x][tile_pos.y]; 
	else
	{
		std::cerr << "Erreur lors de la tentative de l'acquisition d'une tile, la map n'est peut être pas correctement chargée" << std::endl;
		return (Tile){sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	}
}

Tile Map::getTileFromCoords(int height, sf::Vector2f position) const
{
	/* Return a specific tile of the map with the coordinates given in parameters */

	if(m_tiles!=NULL && m_tiles[height]!=NULL && m_tiles[height][(int)(position.x/this->getTileSize())]!=NULL) 
		return m_tiles[height][(int)(position.x/this->getTileSize())][(int)(position.y/this->getTileSize())]; 
	else
	{
		std::cerr << "Erreur lors de la tentative de l'acquisition d'une tile, la map n'est peut être pas correctement chargée" << std::endl;	
		return (Tile){sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	}
}



// Setters

void Map::setSize(unsigned int size_x, unsigned int size_y)
{
	/* Resize the map */

	if(size_x==0 || size_y==0)
		return;

	for(int h=0; h<m_height; h++)
	{
		m_tiles[h] = (Tile**)realloc( m_tiles[h] , size_x * sizeof(Tile*) );
		for(unsigned int i=0; i<size_x; i++)
		{
			if(i < m_size.x)
				m_tiles[h][i] = (Tile*)realloc( m_tiles[h][i] , size_y * sizeof(Tile));
			else
				m_tiles[h][i] = (Tile*)malloc( size_y * sizeof(Tile));

			for(unsigned int j=0; j<size_y; j++)
			{
				if(i>=m_size.x || j>=m_size.y)
				{
					m_tiles[h][i][j].m_pos_text=sf::Vector2f(0,0);
					m_tiles[h][i][j].m_collisionable=false;
				}
			}
		}
	}

	m_size.x = size_x;
	m_size.y = size_y;
}


void Map::setHeight(unsigned int height)
{
	/* Set height to the map */

	if(height==0)
		return;

	m_tiles = (Tile***)realloc( m_tiles , height * sizeof(Tile**));
	for(unsigned int h=m_height; h<height; h++)
	{
		m_tiles[h] = (Tile**)malloc( m_size.x * sizeof(Tile*) );

		for(int i=0; i<m_size.x; i++)
		{
			m_tiles[h][i] = (Tile*)malloc( (int)m_size.y * sizeof(Tile));
			for(int j=0; j<m_size.y; j++)
			{
				m_tiles[h][i][j].m_pos_text=sf::Vector2f(0,0);
				m_tiles[h][i][j].m_collisionable=false;
			}
		}
	}
}


void Map::setTile(Tile tile, sf::Vector2i position, int height)
{
	/* Set a tile given in parameter to a specific position and height in map */
	m_tiles[height][position.x][position.y]=tile;
}



void Map::setTexture(sf::Texture* texture)
{
	/* Set the texture associated with the map */
	m_texture=texture;
}



// Methods

void Map::update()
{
	/* Update the map by updating the map vertex and the time. The entities are sorted in pos_y + size_y order */

	std::sort(m_entities.begin(), m_entities.end(), comparePosY);

	m_time=clock();
	m_vertex.clear();
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

				m_vertex[index].texCoords = m_tiles[h][i][j].m_pos_text + sf::Vector2f(0, 0);
				m_vertex[index+1].texCoords = m_tiles[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, 0);
				m_vertex[index+2].texCoords = m_tiles[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, m_tile_sz);
				m_vertex[index+3].texCoords= m_tiles[h][i][j].m_pos_text + sf::Vector2f(0, m_tile_sz);

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
	/* 	Update such as the normal update except that tiles outside the chosen height are transparent 
	 *	Useful for MapCreator */

	std::sort(m_entities.begin(), m_entities.end(), comparePosY);

	
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

				m_vertex[index].texCoords = m_tiles[h][i][j].m_pos_text + sf::Vector2f(0, 0);
				m_vertex[index+1].texCoords = m_tiles[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, 0);
				m_vertex[index+2].texCoords = m_tiles[h][i][j].m_pos_text + sf::Vector2f(m_tile_sz, m_tile_sz);
				m_vertex[index+3].texCoords= m_tiles[h][i][j].m_pos_text + sf::Vector2f(0, m_tile_sz);

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


void Map::physics_entities()
{
	/* Set on physics between entities, themselves and tiles of the map */

	for(std::vector<Entity*>::iterator ent1=m_entities.begin(); ent1!=m_entities.end(); ent1++)
	{	
		// Physics of entities with themselves
		for(std::vector<Entity*>::iterator ent2=ent1+1; ent2!=m_entities.end(); ent2++)
		{
			if((*ent1)->getType().compare("Character")==0 && (*ent2)->getType().compare("Character")==0)
				physics_characters((Character*)(*ent1), (Character*)(*ent2));
			else if((*ent1)->getType().compare("Character")==0 && (*ent2)->getType().compare("Collector")==0)
				physics_character_collector((Character*)(*ent1), (Collector*)(*ent2));
			else if((*ent1)->getType().compare("Collector")==0 && (*ent2)->getType().compare("Character")==0)
				physics_character_collector((Character*)(*ent2), (Collector*)(*ent1));

		}

		// Physics of entities with map tiles
		if((*ent1)->getType().compare("Character")==0)
		{
			physics_character_map((Character*)(*ent1), this, (*ent1)->getHeight());
		}
		
	}
}


void Map::addEntity(Entity* entity)
{
	/* Add an entity to the map and sort the entities */
	m_entities.push_back(entity);

	std::sort(m_entities.begin(), m_entities.end(), comparePosY);
}
	

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the map and the associated entities according to their position and their height. Indeed, it allows to have a perspective effect */
	states.transform *= getTransform();

	states.texture = m_texture;

	sf::VertexArray tmp_vertex;
	tmp_vertex.setPrimitiveType(sf::Quads);

	// For each level of height
	for(int h=0; h<this->getHeight(); h++)
	{
		std::vector<Entity*> obj_drawn; // Temporary vector of drawn entities (avoid to redraw them)

		// For each line of tiles
		for(int j=0; j<this->getSize().y; j++)
		{

			// Check for each entity 		
			for(std::vector<Entity*>::const_iterator it=m_entities.begin(); it!=m_entities.end(); it++)
			{	
				// If the part of the entity is already drawn
				bool obj_already_drawn=false;

				for(std::vector<Entity*>::iterator it2=obj_drawn.begin(); it2!=obj_drawn.end(); it2++)
				{
					if( (*it)==(*it2) )
						obj_already_drawn = true;
				}
				
				// If the entity is not drawn and it's behind tiles line
				int tmp_val = (*it)->getPosition().y + (*it)->getSize().y;
				if(!obj_already_drawn && (*it)->getHeight() <= h && tmp_val-(h-(*it)->getHeight())*this->getTileSize() < (j+1)*this->getTileSize())
				{
					// Draw the part of the entity in the same level of height
					if(h<this->getHeight()-1)
						(*it)->drawPart((sf::RenderWindow*)&target, h-(*it)->getHeight());
					else
						(*it)->drawPartAndAbove((sf::RenderWindow*)&target, h-(*it)->getHeight());

					//target.draw(*(*it));
					obj_drawn.push_back(*it);
				}
			}
			

			// Draw the tiles line
			tmp_vertex.resize(this->getSize().x * 4);

			for(int i=0; i< this->getSize().x; i++)
			{
				int index_tmp_vertex = i*4;
				int index_vertex = h*this->getSize().x * this->getSize().y * 4 + i*this->getSize().y * 4 + j*4;
				
				tmp_vertex[index_tmp_vertex] = m_vertex[index_vertex];
				tmp_vertex[index_tmp_vertex+1]=m_vertex[index_vertex+1];
				tmp_vertex[index_tmp_vertex+2]=m_vertex[index_vertex+2];
				tmp_vertex[index_tmp_vertex+3]=m_vertex[index_vertex+3];
			}
			target.draw(tmp_vertex, states);
		}
	}
};


bool comparePosY(Entity* ent1, Entity* ent2)
{ 
	// Compare the entity according to the position_x + size_x

	Character* dyn_char1 = dynamic_cast<Character*>(ent1);
	Character* dyn_char2 = dynamic_cast<Character*>(ent2);

	int val1 = (dyn_char1 != nullptr) ? dyn_char1->getAbsHitbox().top + dyn_char1->getAbsHitbox().height : ent1->getPosition().y + ent1->getSize().y;
	int val2 = (dyn_char2 != nullptr) ? dyn_char2->getAbsHitbox().top + dyn_char2->getAbsHitbox().height : ent2->getPosition().y + ent2->getSize().y;

	return val1 < val2;
}