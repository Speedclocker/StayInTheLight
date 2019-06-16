#include "Map.h"
#include "collision.h"
#include "Character.h"



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
	for(int h=0; h<m_height; h++)
	{
		m_map[h] = (Tile**)malloc( (int)m_size.x * sizeof(Tile*) );
		for(int i=0; i<m_size.x; i++)
		{
			m_map[h][i] = (Tile*)malloc( (int)m_size.y * sizeof(Tile));
			for(int j=0; j<m_size.y; j++)
			{
				m_map[h][i][j].m_pos_text=sf::Vector2f(0,0);
				m_map[h][i][j].m_collisionable=false;
			}
		}
	}	

	this->update();
}


	
Map::~Map()
{
	for(int h=0; h<this->getHeight(); h++)
	{
		for(int i=0; i<this->getSize().x; i++)
		{
			if(m_map[h][i]!=NULL)
				free(m_map[h][i]);
		}

		if(m_map[h]!=NULL)
			free(m_map[h]);
	}	
	free(m_map);	
}


//Taille de la map
int Map::getHeight() const										{	return m_height;	}


//Hauteur de la map
sf::Vector2f Map::getSize() const								{	return m_size;		}


//Taille d'une tile
int Map::getTileSize() const									{	return m_tile_sz;	}


//Une tile de la map
Tile Map::getTile(int height, sf::Vector2i tile_pos) const 		
{	
	if(m_map!=NULL && m_map[height]!=NULL && m_map[height][tile_pos.x]!=NULL) 
		return m_map[height][tile_pos.x][tile_pos.y]; 
	else
	{
		std::cerr << "Erreur lors de la tentative de l'acquisition d'une tile, la map n'est peut être pas correctement chargée" << std::endl;
		return (Tile){sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	}
}

Tile Map::getTileFromCoords(int height, sf::Vector2f position) const
{
	if(m_map!=NULL && m_map[height]!=NULL && m_map[height][(int)(position.x/this->getTileSize())]!=NULL) 
		return m_map[height][(int)(position.x/this->getTileSize())][(int)(position.y/this->getTileSize())]; 
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
		m_map[h] = (Tile**)realloc( m_map[h] , size_x * sizeof(Tile*) );
		for(unsigned int i=0; i<size_x; i++)
		{
			if(i < m_size.x)
				m_map[h][i] = (Tile*)realloc( m_map[h][i] , size_y * sizeof(Tile));
			else
				m_map[h][i] = (Tile*)malloc( size_y * sizeof(Tile));

			for(unsigned int j=0; j<size_y; j++)
			{
				if(i>=m_size.x || j>=m_size.y)
				{
					m_map[h][i][j].m_pos_text=sf::Vector2f(0,0);
					m_map[h][i][j].m_collisionable=false;
				}
			}
		}
	}

	m_size.x = size_x;
	m_size.y = size_y;
}


void Map::setHeight(unsigned int height)
{
	if(height==0)
		return;

	m_map = (Tile***)realloc( m_map , height * sizeof(Tile**));
	for(unsigned int h=m_height; h<height; h++)
	{
		m_map[h] = (Tile**)malloc( m_size.x * sizeof(Tile*) );

		for(int i=0; i<m_size.x; i++)
		{
			m_map[h][i] = (Tile*)malloc( (int)m_size.y * sizeof(Tile));
			for(int j=0; j<m_size.y; j++)
			{
				m_map[h][i][j].m_pos_text=sf::Vector2f(0,0);
				m_map[h][i][j].m_collisionable=false;
			}
		}
	}
}

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
	std::sort(m_entities.begin(), m_entities.end(), comparePosY);


	m_time=clock();
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
	/* Useful for MapCreator */
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


void Map::physics_entities()
{
	for(std::vector<Entity*>::iterator ent1=m_entities.begin(); ent1!=m_entities.end(); ent1++)
	{
		
		//Physique des entités entre eux
		for(std::vector<Entity*>::iterator ent2=ent1+1; ent2!=m_entities.end(); ent2++)
		{
			if((*ent1)->getType().compare("Character")==0 && (*ent2)->getType().compare("Character")==0)
				physics_characters((Character*)(*ent1), (Character*)(*ent2));
		}

		//Physique des entités avec les éléments de la map
		physics_character_map((Character*)(*ent1), this, 0);
		
	}
}


void Map::addEntity(Entity* entity)
{
	m_entities.push_back(entity);

	std::sort(m_entities.begin(), m_entities.end(), comparePosY);
}
	



/*
//Draw version simplifiée (sans perspective ou notion d'ordre d'affichage)
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	states.texture = m_texture;

	target.draw(m_vertex, states);
};
*/

/*

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Permet de dessiner la map et les objets qui s'y trouvent dans un ordre dépendant de leur position. En effet, l'ordre d'affichage permet de définir un effet de perspective.
	states.transform *= getTransform();

	states.texture = m_texture;

	sf::VertexArray tmp_vertex;
	tmp_vertex.setPrimitiveType(sf::Quads);

	std::vector<Entity*> obj_drawn;

	for(int j=0; j<this->getSize().y + this->getHeight() - 1; j++)
	{



		///////////////////////////////////
		//Dessine les objets		
		for(std::vector<Entity*>::const_iterator it=m_entities.begin(); it!=m_entities.end(); it++)
		{
			
			bool obj_already_drawn=false;
	
			for(std::vector<Entity*>::iterator it2=obj_drawn.begin(); it2!=obj_drawn.end(); it2++)
			{
				if( (*it)==(*it2) )
					obj_already_drawn = true;
			}

			Character* dyn_char = dynamic_cast<Character*>(*it); // Si l'objet est un personnage, on prend sa hitbox
			int tmp_val = (dyn_char != nullptr) ? dyn_char->getAbsHitbox().top + dyn_char->getAbsHitbox().height : (*it)->getPosition().y + (*it)->getSize().y;

			if(!obj_already_drawn && ((*it)->getPosition().y + (*it)->getSize().y) tmp_val < ( (j) * this->getTileSize()) + 1)
			{
				target.draw(*(*it));
				obj_drawn.push_back(*it);
			}
		}
		//
		////////////////////////////////////



		////////////////////////////////////
		// Dessine la map
		int tmp_size = this->getSize().x * 4;

		if(j>=this->getSize().y)
			tmp_size *= (this->getSize().y + this->getHeight()-1 - j);
		else if(j<this->getHeight())
			tmp_size *= (j+1);
		else
			tmp_size *= this->getHeight();

		tmp_vertex.resize(tmp_size);

		int tmp_j=0;

		for(int h=0; h < this->getHeight(); h++)
		{
			if(j-h>=0 && j-h < this->getSize().y)
			{
				for(int i=0; i< this->getSize().x; i++)
				{
					int index_tmp_vertex = tmp_j*this->getSize().x * 4 + i*4;
					int index_vertex = h*this->getSize().x * this->getSize().y * 4 + i*this->getSize().y * 4 + (j-h)*4;
					
					tmp_vertex[index_tmp_vertex] = m_vertex[index_vertex];
					tmp_vertex[index_tmp_vertex+1]=m_vertex[index_vertex+1];
					tmp_vertex[index_tmp_vertex+2]=m_vertex[index_vertex+2];
					tmp_vertex[index_tmp_vertex+3]=m_vertex[index_vertex+3];
				}

				tmp_j++;
			}
		}

		target.draw(tmp_vertex, states);
		//
		//////////////////////////////////

	}
};
*/

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Permet de dessiner la map et les objets qui s'y trouvent dans un ordre dépendant de leur position. En effet, l'ordre d'affichage permet de définir un effet de perspective.
	states.transform *= getTransform();

	states.texture = m_texture;

	sf::VertexArray tmp_vertex;
	tmp_vertex.setPrimitiveType(sf::Quads);


	for(int h=0; h<this->getHeight(); h++)
	{

		std::vector<Entity*> obj_drawn;

		for(int j=0; j<this->getSize().y; j++)
		{
			///////////////////////////////////
			//Dessine les objets		
			for(std::vector<Entity*>::const_iterator it=m_entities.begin(); it!=m_entities.end(); it++)
			{	
				bool obj_already_drawn=false;

				for(std::vector<Entity*>::iterator it2=obj_drawn.begin(); it2!=obj_drawn.end(); it2++)
				{
					if( (*it)==(*it2) )
						obj_already_drawn = true;
				}
				Character* dyn_char = dynamic_cast<Character*>(*it); // Si l'objet est un personnage, on prend sa hitbox
				int tmp_val = (*it)->getPosition().y + (*it)->getSize().y;
				
				if(!obj_already_drawn && (*it)->getHeight() <= h && tmp_val-(h-(*it)->getHeight())*this->getTileSize() < (j+1)*this->getTileSize())
				{
					if(h<this->getHeight()-1)
						(*it)->drawPart((sf::RenderWindow*)&target, h-(*it)->getHeight());
					else
						(*it)->drawPartAndAbove((sf::RenderWindow*)&target, h-(*it)->getHeight());

					//target.draw(*(*it));
					obj_drawn.push_back(*it);
				}
			}
			//
			////////////////////////////////////

			////////////////////////////////////
			// Dessine la map
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
			//
			//////////////////////////////////
		}
	}
};

/*
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Permet de dessiner la map et les objets qui s'y trouvent dans un ordre dépendant de leur position. En effet, l'ordre d'affichage permet de définir un effet de perspective.
	states.transform *= getTransform();

	states.texture = m_texture;

	sf::VertexArray tmp_vertex;
	tmp_vertex.setPrimitiveType(sf::Quads);

	std::vector<Entity*> obj_drawn;

	for(int j=0; j<this->getSize().y; j++)
	{
		for(int h=0; h<this->getHeight(); h++)
		{
			///////////////////////////////////
			//Dessine les objets		
			for(std::vector<Entity*>::const_iterator it=m_entities.begin(); it!=m_entities.end(); it++)
			{	
				bool obj_already_drawn=false;

				for(std::vector<Entity*>::iterator it2=obj_drawn.begin(); it2!=obj_drawn.end(); it2++)
				{
					if( (*it)==(*it2) )
						obj_already_drawn = true;
				}

				Character* dyn_char = dynamic_cast<Character*>(*it); // Si l'objet est un personnage, on prend sa hitbox
				int tmp_val = (dyn_char != nullptr) ? dyn_char->getAbsHitbox().top + dyn_char->getAbsHitbox().height : (*it)->getPosition().y + (*it)->getSize().y;
			

				if(!obj_already_drawn && (tmp_val + (*it)->getHeight()*this->getTileSize()) <= (j+h+1)*this->getTileSize())
				{
					//target.draw(**it);
					obj_drawn.push_back(*it);
				}
			}
			//
			////////////////////////////////////

			////////////////////////////////////
			// Dessine la map
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
			//
			//////////////////////////////////
		}
	}
};
*/

bool comparePosY(Entity* ent1, Entity* ent2)
{ 
	Character* dyn_char1 = dynamic_cast<Character*>(ent1);
	Character* dyn_char2 = dynamic_cast<Character*>(ent2);

	int val1 = (dyn_char1 != nullptr) ? dyn_char1->getAbsHitbox().top + dyn_char1->getAbsHitbox().height : ent1->getPosition().y + ent1->getSize().y;
	int val2 = (dyn_char2 != nullptr) ? dyn_char2->getAbsHitbox().top + dyn_char2->getAbsHitbox().height : ent2->getPosition().y + ent2->getSize().y;

	return val1 < val2;
}