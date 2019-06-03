#include "InterfaceObject.h"

std::string InterfaceObject::getID()
{
	return m_id;
}

sf::Vector2f InterfaceObject::getPosition()
{
	return m_position;
}

sf::Vector2f InterfaceObject::getSize()
{
	return m_size;
}




void InterfaceObject::setID(std::string id)
{
	m_id = id;
}

void InterfaceObject::setSize(sf::Vector2f size)
{
	m_size=size;
}

void InterfaceObject::setPosition(sf::Vector2f position)
{
	m_position = position;
}


void InterfaceObject::update()
{

}



//////////////////////////////////////////////////////////// TilesetSelect //////////////////////////////////////////////////////////



TilesetSelect::TilesetSelect()
{

}


TilesetSelect::~TilesetSelect()
{
	//free(m_tiles);
}


TilesetSelect::TilesetSelect(const std::string id, sf::Texture* texture, sf::Rect<float> zone, Tile tiles[], int nbr_available_tiles, int size_tile)
{
	m_id = id;
	m_texture = texture;
	m_zone = zone;
	m_size_tile = size_tile;
	m_nbr_tiles = nbr_available_tiles;
	m_tiles = tiles;

	if(!m_state_text.loadFromFile("imgs/state_collision.png")) { std::cerr << "Une erreur a eu lieu lors du chargement de l'image state_collision.png" << std::endl; }

/*
	for(int i=0; i<m_nbr_tiles; i++)
	{
		m_tiles[i]=tiles[i];
	}
	*/
}


sf::Vector2f TilesetSelect::getPosition()
{
	return m_position;
}


sf::Vector2f TilesetSelect::getSize()
{
	return sf::Vector2f(this->getZone().width, this->getZone().height);
}


sf::Rect<float> TilesetSelect::getZone()
{
	return m_zone;
}


sf::Vector2f TilesetSelect::getMaxZonePos()
{
	return sf::Vector2f(m_texture->getSize().x - m_zone.width, m_texture->getSize().y - m_zone.height);
}


Tile TilesetSelect::getChosenTile()
{
	return m_tiles[m_chosen_tile];
}



void TilesetSelect::setPosition(sf::Vector2f position)
{
	m_position = position;
}

/*
void TilesetSelect::setSize(sf::Vector2f size)
{
	m_size = size;
}
*/

void TilesetSelect::setZone(sf::Rect<float> zone)
{
	sf::Rect<float> tmp_zone = zone;
	if(tmp_zone.left + tmp_zone.width > m_texture->getSize().x)
		tmp_zone.left = m_texture->getSize().x - tmp_zone.width;

	if(tmp_zone.top + tmp_zone.height > m_texture->getSize().y)
		tmp_zone.top = m_texture->getSize().y - tmp_zone.height;

	if(tmp_zone.left < 0)
	{
		tmp_zone.left = 0;
		tmp_zone.width = m_texture->getSize().x;
	}

	if(tmp_zone.top < 0)
	{
		tmp_zone.top = 0;
		tmp_zone.height = m_texture->getSize().y;
	}

	m_zone = tmp_zone;
}



void TilesetSelect::collision_settings(sf::RenderWindow* window)
{
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	sf::Vector2f positionTile((int)((mousePos.x - this->getPosition().x + this->getZone().left)/m_size_tile)*m_size_tile,
							  (int)((mousePos.y - this->getPosition().y + this->getZone().top) /m_size_tile)*m_size_tile);

	
	if( !sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
	 	mousePos.x >= this->getPosition().x && mousePos.x < this->getPosition().x + this->getSize().x && 
		mousePos.y >= this->getPosition().y && mousePos.y < this->getPosition().y + this->getSize().y )
	{
		m_state = HOVER;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = NONE;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == HOVER)
	{
		m_state = CLICK;
		int index=(int)(positionTile.x/m_size_tile) + (int)(positionTile.y/m_size_tile) * (int)(m_texture->getSize().x/m_size_tile) ;

		if(index < m_nbr_tiles)
			m_tiles[index].m_collisionable^=true;
	}
}



void TilesetSelect::choice_tile(sf::RenderWindow* window)
{
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	sf::Vector2f positionTile((int)((mousePos.x - this->getPosition().x + this->getZone().left)/m_size_tile)*m_size_tile,
							  (int)((mousePos.y - this->getPosition().y + this->getZone().top) /m_size_tile)*m_size_tile);

	//Fonction qui renvoie le tile choisi
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
		mousePos.x >= this->getPosition().x && mousePos.x < this->getPosition().x + this->getSize().x && 
		mousePos.y >= this->getPosition().y && mousePos.y < this->getPosition().y + this->getSize().y )
	{
		m_state = HOVER;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = NONE;


	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == HOVER)
	{
		m_state = CLICK;
		int index=(int)(positionTile.x/m_size_tile) + (int)(positionTile.y/m_size_tile) * (int)(m_texture->getSize().x/m_size_tile) ;

		if(index < m_nbr_tiles)
		{
			m_chosen_tile_pos_text = positionTile;
			m_chosen_tile = index;
		}
	}


}


void TilesetSelect::interactsWithUser(sf::RenderWindow* window)
{
	if(m_nbr_tiles > 0) 
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
			this->collision_settings(window);
		else
			this->choice_tile(window);
	}
}


void TilesetSelect::update()
{
	m_vertex.setPrimitiveType(sf::Quads);

	m_vertex.resize(4);

	m_vertex[0].position = m_position;
	m_vertex[1].position = m_position + sf::Vector2f( m_zone.width, 0);
	m_vertex[2].position = m_position + sf::Vector2f( m_zone.width, m_zone.height );
	m_vertex[3].position = m_position + sf::Vector2f( 0, m_zone.height );


	m_vertex[0].texCoords = sf::Vector2f(m_zone.left, m_zone.top);
	m_vertex[1].texCoords = sf::Vector2f(m_zone.left + m_zone.width , m_zone.top);
	m_vertex[2].texCoords = sf::Vector2f(m_zone.left + m_zone.width, m_zone.top + m_zone.height);
	m_vertex[3].texCoords = sf::Vector2f(m_zone.left, m_zone.top + m_zone.height);
}


void TilesetSelect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	//Dessin du fond du tileset
	sf::RectangleShape background_tileset(sf::Vector2f(m_zone.width, m_zone.height));
	background_tileset.setFillColor(sf::Color(10,10,10,180));
	background_tileset.setPosition(m_position);
	target.draw(background_tileset, states);




	//Dessin du tileset
	states.texture = m_texture;
	target.draw(m_vertex, states);




	//Dessin du rectangle de sélection
	if(m_nbr_tiles > 0)
	{
		if(m_chosen_tile_pos_text.x - m_zone.left + m_size_tile > 0 && m_chosen_tile_pos_text.x - m_zone.left < m_zone.width)
		{
			sf::RectangleShape choice_square(m_tiles[0].m_size_text);
			choice_square.setFillColor(sf::Color(0,0,0,0));
			choice_square.setOutlineColor(sf::Color(255,0,0,255));
			choice_square.setOutlineThickness(2);

			sf::Rect<float> tmp_rect( m_position + m_chosen_tile_pos_text - sf::Vector2f(m_zone.left, m_zone.top), choice_square.getSize());

			if(tmp_rect.left < m_position.x)
			{
				tmp_rect.left = m_position.x;
				tmp_rect.width = m_chosen_tile_pos_text.x - m_zone.left + m_size_tile;
			}

			if(tmp_rect.left + tmp_rect.width > m_position.x + m_zone.width)
				tmp_rect.width = m_position.x + m_zone.width - tmp_rect.left;


			choice_square.setPosition(tmp_rect.left, tmp_rect.top);
			choice_square.setSize(sf::Vector2f(tmp_rect.width, tmp_rect.height));

			target.draw(choice_square, states);
		}
	}




	//Montre l'état de collision des tiles
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		for(int i=0; i<m_nbr_tiles; i++)
		{
			if( -m_zone.left + m_tiles[i].m_pos_text.x + m_tiles[i].m_size_text.x > 0 && -m_zone.left + m_tiles[i].m_pos_text.x < m_zone.width)
			{
				sf::VertexArray state_collision_bloc(sf::Quads, 4);

				sf::Rect<float> tmp_rect( m_position + m_tiles[i].m_pos_text - sf::Vector2f(m_zone.left, m_zone.top), m_tiles[i].m_size_text );

				if(tmp_rect.left < m_position.x)
				{
					tmp_rect.left = m_position.x;
					tmp_rect.width = m_tiles[i].m_pos_text.x - m_zone.left + m_tiles[i].m_size_text.x;
				}

				if(tmp_rect.left + tmp_rect.width > m_position.x + m_zone.width)
					tmp_rect.width = m_position.x + m_zone.width - tmp_rect.left;

				state_collision_bloc[0].position	=	sf::Vector2f(tmp_rect.left, tmp_rect.top);
				state_collision_bloc[1].position 	= 	sf::Vector2f(tmp_rect.left, tmp_rect.top) + sf::Vector2f(tmp_rect.width, 0);
				state_collision_bloc[2].position 	= 	sf::Vector2f(tmp_rect.left, tmp_rect.top) + sf::Vector2f(tmp_rect.width, tmp_rect.height);
				state_collision_bloc[3].position 	= 	sf::Vector2f(tmp_rect.left, tmp_rect.top) + sf::Vector2f(0, tmp_rect.height);

				//Si il s'agit d'une croix, collisionable, sinon, non collisionable
				sf::Vector2f offset_texture( (m_tiles[i].m_collisionable) ? m_state_text.getSize().x/2 : 0 , 0);

				state_collision_bloc[0].texCoords	=	sf::Vector2f(0, 0) + offset_texture;
				state_collision_bloc[1].texCoords 	= 	sf::Vector2f(m_state_text.getSize().x/2, 0) + offset_texture;
				state_collision_bloc[2].texCoords 	= 	sf::Vector2f(m_state_text.getSize().x/2, m_state_text.getSize().y) + offset_texture;
				state_collision_bloc[3].texCoords 	= 	sf::Vector2f(0, m_state_text.getSize().y) + offset_texture;

				target.draw(state_collision_bloc, &m_state_text);
			}
		}
	}


}




//////////////////////////////////////////////////////////// SLIDEBAR //////////////////////////////////////////////////////////



SlideBar::SlideBar()
{

}


SlideBar::SlideBar(std::string id, float minValue, float maxValue)
{
	m_id = id;
	m_minValue = minValue;
	m_maxValue = maxValue;
	if(m_maxValue <= m_minValue)
		m_maxValue = m_minValue + 1;

	m_currentValue = minValue;
}


SlideBar::~SlideBar()
{

}


// Accesseurs
float SlideBar::getMinValue()
{
	return m_minValue;
}

float SlideBar::getMaxValue()
{
	return m_maxValue;
}

float SlideBar::getCurrentValue()
{
	return m_currentValue;
}

SlideBar::Type SlideBar::getType()
{
	return m_type;
}


//Modificateurs
void SlideBar::setMinValue(float minValue)
{
	if(m_maxValue > minValue)	
		m_minValue = minValue;
	else
		m_minValue = m_maxValue - 1;
}

void SlideBar::setMaxValue(float maxValue)
{
	if(maxValue > m_minValue)
		m_maxValue = maxValue;
	else
		m_maxValue = m_minValue + 1;
}

void SlideBar::setCurrentValue(float currentValue)
{
	m_currentValue = currentValue;
}

void SlideBar::setCurrentValueFromPos(sf::Vector2f clickPos)
{
	if(this->getType()==HORIZONTAL)
	{
		this->setCurrentValue( this->getMinValue() + (this->getMaxValue() - this->getMinValue())*(clickPos.x - this->getPosition().x)/this->getSize().x );
		this->update();
	}
	else
	{
		this->setCurrentValue( this->getMinValue() + (this->getMaxValue() - this->getMinValue())*(clickPos.y - this->getPosition().y)/this->getSize().y );
		this->update();
	}
}

void SlideBar::setType(Type type)
{
	m_type = type;
}



//Méthodes
void SlideBar::interactsWithUser(sf::RenderWindow* window)
{
	// Interaction avec l'utilisateur
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect((sf::Vector2i)this->getPosition(), (sf::Vector2i)this->getSize()).contains(mousePos.x, mousePos.y))
		m_state = HOVER;
	else if(m_state==HOVER && sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		m_state = CLICK;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = NONE;

	if(m_state == CLICK)
	{
		this->setCurrentValueFromPos(mousePos);
	}

	this->update();
}

void SlideBar::update()
{
	if(m_currentValue > m_maxValue)	
		m_currentValue = m_maxValue;

	if(m_currentValue < m_minValue)	
		m_currentValue = m_minValue;

}


void SlideBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape zone_bar, sliding_bar;
	zone_bar.setFillColor(sf::Color(50,50,50,180));
	zone_bar.setOutlineThickness(2);
	zone_bar.setOutlineColor(sf::Color(40,40,40,200));
	zone_bar.setSize(m_size);
	zone_bar.setPosition(m_position);

	sliding_bar.setFillColor(sf::Color(150, 150, 150, 225));

	float coeff_size = 1./(m_maxValue*0.01-m_minValue*0.01);
	if(m_size.x * coeff_size > m_size.x) coeff_size = 1;
	if(m_size.x * coeff_size < m_size.x * 1./20.) coeff_size = 1./20.;

	if(m_type==HORIZONTAL)
	{
		sliding_bar.setSize(sf::Vector2f(m_size.x * coeff_size, m_size.y));
		sliding_bar.setPosition(m_position.x + (m_size.x - sliding_bar.getSize().x)*(m_currentValue - m_minValue)/(m_maxValue - m_minValue), m_position.y);
	}
	else
	{
		sliding_bar.setSize(sf::Vector2f(m_size.x, m_size.y*coeff_size));
		sliding_bar.setPosition(m_position.x , m_position.y + (m_size.y - sliding_bar.getSize().y)*(m_currentValue - m_minValue)/(m_maxValue - m_minValue));
	}

	target.draw(zone_bar, states);
	target.draw(sliding_bar, states);
}




