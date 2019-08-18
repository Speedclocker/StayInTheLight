#include "InterfaceObject.h"
#include "Animation.h"

#define FONT_FILE "data/fonts/AldoTheApache.ttf"
#define SCROLLINGLIST_HEIGHT_COEFF 1.5
#define SCROLLINGLIST_WIDTH_COEFF 1.3





//////////////////////////////////////////////////////////////// InterfaceObject /////////////////////////////////////////////////////////////


InterfaceObject::InterfaceObject()
{

}

InterfaceObject::~InterfaceObject()
{

}



std::string InterfaceObject::getID()
{
	// Return the id of the object
	return m_id;
}

sf::Vector2f InterfaceObject::getPosition()
{
	// Return the position of the object
	return m_position;
}


sf::Rect<float> InterfaceObject::getInteractableZone()
{
	return sf::Rect<float>(m_position, m_size);
}

sf::Vector2f InterfaceObject::getSize()
{
	// Return the size of the object
	return m_size;
}

InterfaceObject::State InterfaceObject::getState()
{
	// Return the size of the object
	return m_state;
}

bool InterfaceObject::isOverall()
{
	// Return the size of the object
	return m_overall;
}



void InterfaceObject::setID(std::string id)
{
	// Set the id of the object
	m_id = id;
}

void InterfaceObject::setSize(sf::Vector2f size)
{
	// Set the size of the object
	m_size=size;
}

void InterfaceObject::setPosition(sf::Vector2f position)
{
	// Set the position of the object
	m_position = position;
}

void InterfaceObject::setState(InterfaceObject::State state)
{
	m_state = state;
}



void InterfaceObject::update()
{
	// Update the object
}


void InterfaceObject::interactsWithUser(sf::RenderWindow* window)
{
	window->getSize();
}


////////////////////////////////////////////////////////////// InputBar /////////////////////////////////////////////////////////////



InputBar::InputBar()
{
	/* Constructor of the object */
	//m_buff = new std::string("");;
}


InputBar::~InputBar()
{
	/* Destructor of the object */
	if(m_ptr_event_txt_entered_associated != NULL && *m_ptr_event_txt_entered_associated == m_buff)
		*m_ptr_event_txt_entered_associated = NULL;

	delete m_buff;

}

InputBar::InputBar(std::string id, int size_font, int size_long, InputBar::Type type, std::string** ptr_event_txt_entered_associated)
{
	/* Constructor of the object */

	m_id = id;
	m_state = InterfaceObject::NONE;
	m_overall = false;
	m_default_text = "";
	m_size_font = size_font;
	m_type = type;
	m_buff = new std::string("");
	m_size = sf::Vector2f(10,10);
	m_focus = false;
	m_time = clock();
	m_typing_cursor = true;


	m_ptr_event_txt_entered_associated = ptr_event_txt_entered_associated;
	

	m_size = sf::Vector2f(size_long, m_size_font*1.5);

	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;

}



InputBar::InputBar(std::string id, int size_font, int size_long, InputBar::Type type, std::string** ptr_event_txt_entered_associated, std::string default_text)
{
	/* Constructor of the object */
	m_id = id;
	m_state = InterfaceObject::NONE;
	m_overall = false;
	m_default_text = default_text;
	m_size_font = size_font;
	m_type = type;
	m_buff = new std::string("");
	m_size = sf::Vector2f(10,10);
	m_focus = false;
	m_time = clock();
	m_typing_cursor = true;

	m_ptr_event_txt_entered_associated = ptr_event_txt_entered_associated;
	

	m_size = sf::Vector2f(size_long, m_size_font*1.5);

	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;


}





// Getters

sf::Vector2f InputBar::getPosition()
{
	/* Return the position */
	return m_position;
}


sf::Vector2f InputBar::getSize()
{
	/* Return the size */
	return m_size;
}


std::string InputBar::getValue()
{
	/* Return the value of the buffer */
	return *m_buff;
}


InputBar::Type InputBar::getType()
{
	/* Return the type */
	return m_type;
}


//Setters

void InputBar::setPosition(sf::Vector2f position)
{
	/* Set the position */
	m_position = position;
}


void InputBar::setSize(sf::Vector2f size)
{
	/* Set the size */
	m_size = size;
}


void InputBar::setType(InputBar::Type type)
{
	/* Set the type */
	m_type = type;
}


void  InputBar::setValue(std::string buff)
{
	/* Set a value (string) to the buffer */
	*m_buff = buff;
	if(this->getType() == InputBar::NUMERICAL)
	{
		std::string::iterator it = m_buff->begin();
		while(it != m_buff->end())
		{
			if(((*m_buff)[it-m_buff->begin()] < 48 || (*m_buff)[it-m_buff->begin()] > 57))
				it = m_buff->erase(it);
			else
				it++;
		}
	}
	else if(this->getType() == InputBar::ALPHANUMERICAL)
	{
		std::string::iterator it = m_buff->begin();
		while(it != m_buff->end())
		{
			if((*m_buff)[it-m_buff->begin()] < 48 || ( ((*m_buff)[it-m_buff->begin()] > 57) && (*m_buff)[it-m_buff->begin()] < 65 ) || ( ((*m_buff)[it-m_buff->begin()] > 90) && (*m_buff)[it-m_buff->begin()] < 97 ) || ((*m_buff)[it-m_buff->begin()] > 122))
				it = m_buff->erase(it);
			else
				it++;
		}
	}
	else if(this->getType() == InputBar::ALPHANUMERICAL_SPEC_CHARACTERS)
	{
		std::string::iterator it = m_buff->begin();
		while(it != m_buff->end())
		{
			if( (*m_buff)[it-m_buff->begin()] < 32 || ( ((*m_buff)[it-m_buff->begin()] > 126)))
				it = m_buff->erase(it);
			else
				it++;
		}
	}
}

//Methods

bool InputBar::hasFocus()
{
	/* Return true if inputbar has focus or false otherwise */
	return m_focus;
}



void InputBar::interactsWithUser(sf::RenderWindow* window)
{
	/* Function to interact with the user and set the focus if user clicks on the bar */

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Rect<float>(this->getPosition(), this->getSize()).contains(mousePos))
	{
		m_state = InterfaceObject::HOVER;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_state = InterfaceObject::NONE;
	}
	else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == InterfaceObject::HOVER)
	{
		m_state = InterfaceObject::CLICK;
		m_focus = true;

		if(m_ptr_event_txt_entered_associated != NULL)
			*m_ptr_event_txt_entered_associated = m_buff;
	}
	

	// Set the focus if the user clicks on the bar
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Rect<float>(this->getPosition(), this->getSize()).contains(mousePos))
	{
		m_focus = false;
	}
	
}


void InputBar::update()
{
	/* Function updating the bar. If the bar has the focus, the buffer is associated to the TextEnter event pointer */
	

	
	// Set the TextEnter event pointer
	if(this->hasFocus())
	{
		if(m_ptr_event_txt_entered_associated != NULL && *m_ptr_event_txt_entered_associated != m_buff)
			m_focus = false;
	}
	else
	{
		if(m_ptr_event_txt_entered_associated != NULL && *m_ptr_event_txt_entered_associated == m_buff)
			*m_ptr_event_txt_entered_associated = NULL;
	}
		


	// Check the type 
	if(this->getType()==InputBar::NUMERICAL)
	{
		if( m_buff->length() > 0 && ((*m_buff)[m_buff->length()-1] < 48 || (*m_buff)[m_buff->length()-1] > 57) ) 
			m_buff->erase(m_buff->end()-1);
	}

	// Blinking the typing cursor
	if(!m_focus) m_time = clock();
	else if( clock() - m_time > 0.5 * CLOCKS_PER_SEC ) { m_typing_cursor^=true; m_time = clock(); }
}


void InputBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the bar*/

	// Draw the appearance of the bar
	sf::RectangleShape appearance_input(m_size);
	appearance_input.setOutlineThickness(1);
	appearance_input.setOutlineColor(sf::Color(245, 245, 245, 245));
	appearance_input.setFillColor(sf::Color(50, 50, 50, 180));
	appearance_input.setPosition(m_position);
	target.draw(appearance_input, states);

	// Draw the text

	sf::Text text_input;
	text_input.setCharacterSize(m_size_font);
	text_input.setFont(m_font);

	if(strcmp(m_buff->c_str(), "")==0 && !m_focus)
	{
		text_input.setString(m_default_text);
		text_input.setFillColor(sf::Color(150,150,150,180));
	} 	
	else
		text_input.setString(*m_buff);

	text_input.setOrigin(text_input.getLocalBounds().left, text_input.getLocalBounds().top);
	text_input.setPosition((int)(m_position.x + m_size.x/2 - text_input.getLocalBounds().width/2), (int)(m_position.y  + m_size.y/2 - text_input.getLocalBounds().height/2));

	
	target.draw(text_input, states);

	
	if(m_focus && m_typing_cursor)
	{
		// Draw the typing cursor if it has the cursor
		sf::RectangleShape typing_bar(sf::Vector2f(2, m_size_font));
		typing_bar.setFillColor(sf::Color::White);
		typing_bar.setPosition(text_input.getPosition().x + text_input.getLocalBounds().width + 3, m_position.y + m_size.y/2 - typing_bar.getSize().y/2);
		target.draw(typing_bar);

	}
}



////////////////////////////////////////////////////////////// Button ///////////////////////////////////////////////////////////////



Button::Button()
{
	/* Constructor of the object */
}



Button::~Button()
{
	/* Destructor of the object */
}



Button::Button(std::string id, Button::Type type, std::string text, int size_font)
{
	/* Constructor of the object */

	m_id = id;
	m_overall = false;
	m_type = type;
	m_text = text;
	m_size_font = size_font;
	m_state = InterfaceObject::NONE;
	m_isClicked = false;

	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	else
	{
		sf::Text tmp_char(m_text, m_font, m_size_font);		
		m_size = sf::Vector2f((tmp_char.getLocalBounds().width * 1.3), (tmp_char.getLocalBounds().height * 2.4));
	}
}



// Getters
sf::Vector2f Button::getPosition()
{
	/* Return the position */

	return m_position;
}


sf::Vector2f Button::getSize()
{
	/* Return the size */

	return m_size;
}


Button::Type Button::getType()
{
	/* Return the type */

	return m_type;
}



// Setters

void Button::setPosition(sf::Vector2f position)
{
	/* Set the position */
	m_position = position;
}



void Button::setSize(sf::Vector2f size)
{
	/* Set the size */
	m_size = size;
}


void Button::setType(Button::Type type)
{
	m_type = type;
}


void Button::setText(std::string text)
{
	m_text = text;
}



// Methods

void Button::click()
{
	if(this->getType() == Button::ONE_STATE) m_isClicked = true;
	else if(this->getType() == Button::TWO_STATES) m_isClicked^=true;
}



bool Button::isClicked()
{
	/* Returns true if the button is clicked (rising edge) */
	return m_isClicked;
}


void Button::interactsWithUser(sf::RenderWindow* window)
{
	/* Function to interact with the user*/
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Rect<float>(this->getPosition(), this->getSize()).contains(mousePos) && m_state != InterfaceObject::CLICK)
	{
		m_state = InterfaceObject::HOVER;
		if(this->getType() == Button::ONE_STATE)
			m_isClicked = false;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == InterfaceObject::CLICK)
	{
		m_state = InterfaceObject::NONE;
		if(this->getType() == Button::ONE_STATE)
			m_isClicked = true;
		else if(this->getType() == Button::TWO_STATES)
			m_isClicked^=true;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_state = InterfaceObject::NONE;
		if(this->getType() == Button::ONE_STATE)
			m_isClicked = false;
	}
	else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Rect<float>(this->getPosition(), this->getSize()).contains(mousePos) && m_state == InterfaceObject::HOVER)
	{
		m_state = InterfaceObject::CLICK;
		if(this->getType() == Button::ONE_STATE)
			m_isClicked = false;
	}
	else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Rect<float>(this->getPosition(), this->getSize()).contains(mousePos) && m_state == InterfaceObject::CLICK)
	{
		m_state = InterfaceObject::NONE;
		if(this->getType() == Button::ONE_STATE)
			m_isClicked = false;
	}

		


	//if(m_state!=CLICK)
	//	m_isClick = false;
	
	

}



void Button::update()
{
	/* Function to update the button */
}



void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the button */

	// Draw the appearance depending on the state
	sf::RectangleShape appearance_button(m_size);
	if(m_state == InterfaceObject::HOVER)
		appearance_button.setFillColor(sf::Color(180, 180, 180, 225));
	else if(m_state == InterfaceObject::CLICK || m_isClicked)
		appearance_button.setFillColor(sf::Color(225, 225, 225, 225));
	else
		appearance_button.setFillColor(sf::Color(140, 140, 140, 225));
	appearance_button.setPosition(m_position);
	target.draw(appearance_button, states);


	// Draw the text on the button
	sf::Text text_button(m_text, m_font, m_size_font);	
	text_button.setOrigin(text_button.getLocalBounds().left, text_button.getLocalBounds().top);
	text_button.setOutlineThickness(1);
	text_button.setOutlineColor(sf::Color::Black);
	text_button.setPosition((int)(m_position.x + m_size.x/2 - text_button.getLocalBounds().width/2), (int)(m_position.y  + m_size.y/2 - text_button.getLocalBounds().height/2 ));
	target.draw(text_button);

}



//////////////////////////////////////////////////////////// TilesetSelect //////////////////////////////////////////////////////////



TilesetSelect::TilesetSelect()
{
	/* Constructor of the object */
}


TilesetSelect::~TilesetSelect()
{
	/* Destructor of the object */
}


TilesetSelect::TilesetSelect(const std::string id, const sf::Texture* texture, sf::Rect<float> zone, Tile** tiles, const int* nbr_available_tiles, const int* size_tile, Tile* target_tile)
{
	/* Constructor of the object */
	m_id = id;
	m_overall = false;
	m_zone = zone;

	// Association pointers
	m_texture = texture;
	m_size_tile = size_tile;
	m_nbr_tiles = nbr_available_tiles;
	m_ptr_tiles = tiles;
	m_target_tile = target_tile;
	m_state = InterfaceObject::NONE;


	//Initialize values
	m_chosen_tile_pos_text = m_target_tile->m_pos_text;
	m_chosen_tile = (int)(m_chosen_tile_pos_text.x/this->getTileSize()) + (int)(m_chosen_tile_pos_text.y/this->getTileSize()) * (int)(m_texture->getSize().x/this->getTileSize());

	//Load the collision states texture
	if(!m_state_texture.loadFromFile("data/imgs/state_collision.png")) { std::cerr << "Une erreur a eu lieu lors du chargement de l'image state_collision.png" << std::endl; }
}



// Getters


sf::Vector2f TilesetSelect::getPosition()
{
	/* Return the position */
	return m_position;
}


sf::Vector2f TilesetSelect::getSize()
{
	/* Return the size */
	return sf::Vector2f(this->getZone().width, this->getZone().height);
}


sf::Rect<float> TilesetSelect::getZone()
{
	/* Return the zone which is the Rect of the texture which will be drawn */
	return m_zone;
}


sf::Vector2f TilesetSelect::getMaxZonePos()
{
	/* Return the maximum position which is allowed for the zone to be depending on its size */
	return sf::Vector2f(m_texture->getSize().x - m_zone.width, m_texture->getSize().y - m_zone.height);
}


Tile TilesetSelect::getChosenTile()
{
	/* Return the chosen tile */
	return (*m_ptr_tiles)[m_chosen_tile];
}

int TilesetSelect::getTileSize()
{
	/* Return the tile size */
	return *m_size_tile;
}



// Setters

void TilesetSelect::setPosition(sf::Vector2f position)
{
	/* Set the position */
	m_position = position;
}


void TilesetSelect::setTexture(const sf::Texture* texture)
{
	/* Set the texture */
	m_texture = texture;
}



void TilesetSelect::setSize(sf::Vector2f size)
{
	/* Set the size */
	m_size = size;
}


void TilesetSelect::setZone(sf::Rect<float> zone)
{
	/* Set the zone which is the Rect of the texture which will be drawn */

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



void TilesetSelect::setSizeTile(int* size_tile)
{
	/* Set a new pointer to the size tile */

	m_size_tile = size_tile;
}



// Methods

void TilesetSelect::collision_settings(sf::RenderWindow* window)
{
	/* Configure the collision of a tile by clicking on it */

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	sf::Vector2f positionTile((int)((mousePos.x - this->getPosition().x + this->getZone().left)/this->getTileSize())*this->getTileSize(),
							  (int)((mousePos.y - this->getPosition().y + this->getZone().top) /this->getTileSize())*this->getTileSize());

	
	if( !sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
	 	mousePos.x >= this->getPosition().x && mousePos.x < this->getPosition().x + this->getSize().x && 
		mousePos.y >= this->getPosition().y && mousePos.y < this->getPosition().y + this->getSize().y )
	{
		m_state = InterfaceObject::HOVER;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = InterfaceObject::NONE;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == InterfaceObject::HOVER)
	{
		m_state = InterfaceObject::CLICK;
		int index=(int)(positionTile.x/this->getTileSize()) + (int)(positionTile.y/this->getTileSize()) * (int)(m_texture->getSize().x/this->getTileSize()) ;

		if(index < *m_nbr_tiles)
			(*m_ptr_tiles)[index].m_collisionable^=true;
	}
}



void TilesetSelect::choice_tile(sf::RenderWindow* window)
{
	/* Allows the user to choose a tile depending on the place where he is clicking */

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	sf::Vector2f positionTile((int)((mousePos.x - this->getPosition().x + this->getZone().left)/this->getTileSize())*this->getTileSize(),
							  (int)((mousePos.y - this->getPosition().y + this->getZone().top) /this->getTileSize())*this->getTileSize()); // Coords of the tile targeted by the mouse



	/* Change the mouse state (either hover, or click or none) */
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
		mousePos.x >= this->getPosition().x && mousePos.x < this->getPosition().x + this->getSize().x && 
		mousePos.y >= this->getPosition().y && mousePos.y < this->getPosition().y + this->getSize().y )
	{
		m_state = InterfaceObject::HOVER;
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = InterfaceObject::NONE;


	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == InterfaceObject::HOVER)
	{
		m_state = InterfaceObject::CLICK;
		int index=(int)(positionTile.x/this->getTileSize()) + (int)(positionTile.y/this->getTileSize()) * (int)(m_texture->getSize().x/this->getTileSize()) ; // Calculate the index of the tile depending on the click position

		if(index < *m_nbr_tiles)
		{
			// Change the coords variables
			m_chosen_tile_pos_text = positionTile;
			m_chosen_tile = index;
		}
	}


}


void TilesetSelect::interactsWithUser(sf::RenderWindow* window)
{
	/* Allows the user to interact with the tileset by choosing a target tile tile or by changing its collision settings with LAlt*/
	if(*m_nbr_tiles > 0) 
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) 
			this->collision_settings(window);
		else
			this->choice_tile(window);
	}
}


void TilesetSelect::update()
{
	/* Update the vertex tileset and the coords ot the target tile */
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

	m_chosen_tile_pos_text = m_target_tile->m_pos_text;
	m_chosen_tile = (int)(m_chosen_tile_pos_text.x/this->getTileSize()) + (int)(m_chosen_tile_pos_text.y/this->getTileSize()) * (int)(m_texture->getSize().x/this->getTileSize()) ;

}


void TilesetSelect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Function to draw the TilesetSelector and its differents components */

	//Draw the background of the tileset
	sf::RectangleShape background_tileset(sf::Vector2f(m_zone.width, m_zone.height));
	background_tileset.setFillColor(sf::Color(10,10,10,180));
	background_tileset.setPosition(m_position);
	target.draw(background_tileset, states);


	//Draw the tileset
	states.texture = m_texture;
	target.draw(m_vertex, states);


	//Draw the selection rectangle
	if(*m_nbr_tiles > 0)
	{
		if(m_chosen_tile_pos_text.x - m_zone.left + (*m_size_tile) > 0 && m_chosen_tile_pos_text.x - m_zone.left < m_zone.width)
		{
			sf::RectangleShape choice_square((*m_ptr_tiles)[0].m_size_text);
			choice_square.setFillColor(sf::Color(0,0,0,0));
			choice_square.setOutlineColor(sf::Color(255,0,0,255));
			choice_square.setOutlineThickness(2);

			sf::Rect<float> tmp_rect( m_position + m_chosen_tile_pos_text - sf::Vector2f(m_zone.left, m_zone.top), choice_square.getSize());

			if(tmp_rect.left < m_position.x)
			{
				tmp_rect.left = m_position.x;
				tmp_rect.width = m_chosen_tile_pos_text.x - m_zone.left + (*m_size_tile);
			}

			if(tmp_rect.left + tmp_rect.width > m_position.x + m_zone.width)
				tmp_rect.width = m_position.x + m_zone.width - tmp_rect.left;


			choice_square.setPosition(tmp_rect.left, tmp_rect.top);
			choice_square.setSize(sf::Vector2f(tmp_rect.width, tmp_rect.height));

			target.draw(choice_square, states);
		}
	}




	//Shows the collision state of each tile
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		sf::RectangleShape tmp_collision_rect(sf::Vector2f(m_zone.width, m_zone.height));
		tmp_collision_rect.setPosition(m_position);
		tmp_collision_rect.setFillColor(sf::Color(25, 25, 25, 180));
		target.draw(tmp_collision_rect);

		for(int i=0; i<*m_nbr_tiles; i++)
		{
			if( -m_zone.left + (*m_ptr_tiles)[i].m_pos_text.x + (*m_ptr_tiles)[i].m_size_text.x > 0 && -m_zone.left + (*m_ptr_tiles)[i].m_pos_text.x < m_zone.width)
			{
				sf::VertexArray state_collision_bloc(sf::Quads, 4);

				sf::Rect<float> tmp_rect( m_position + (*m_ptr_tiles)[i].m_pos_text - sf::Vector2f(m_zone.left, m_zone.top), (*m_ptr_tiles)[i].m_size_text );

				if(tmp_rect.left < m_position.x)
				{
					tmp_rect.left = m_position.x;
					tmp_rect.width = (*m_ptr_tiles)[i].m_pos_text.x - m_zone.left + (*m_ptr_tiles)[i].m_size_text.x;
				}

				if(tmp_rect.left + tmp_rect.width > m_position.x + m_zone.width)
					tmp_rect.width = m_position.x + m_zone.width - tmp_rect.left;

				state_collision_bloc[0].position	=	sf::Vector2f(tmp_rect.left, tmp_rect.top);
				state_collision_bloc[1].position 	= 	sf::Vector2f(tmp_rect.left, tmp_rect.top) + sf::Vector2f(tmp_rect.width, 0);
				state_collision_bloc[2].position 	= 	sf::Vector2f(tmp_rect.left, tmp_rect.top) + sf::Vector2f(tmp_rect.width, tmp_rect.height);
				state_collision_bloc[3].position 	= 	sf::Vector2f(tmp_rect.left, tmp_rect.top) + sf::Vector2f(0, tmp_rect.height);

				//Si il s'agit d'une croix, collisionable, sinon, non collisionable
				sf::Vector2f offset_texture( ((*m_ptr_tiles)[i].m_collisionable) ? m_state_texture.getSize().x/2 : 0 , 0);

				state_collision_bloc[0].texCoords	=	sf::Vector2f(0, 0) + offset_texture;
				state_collision_bloc[1].texCoords 	= 	sf::Vector2f(m_state_texture.getSize().x/2, 0) + offset_texture;
				state_collision_bloc[2].texCoords 	= 	sf::Vector2f(m_state_texture.getSize().x/2, m_state_texture.getSize().y) + offset_texture;
				state_collision_bloc[3].texCoords 	= 	sf::Vector2f(0, m_state_texture.getSize().y) + offset_texture;

				target.draw(state_collision_bloc, &m_state_texture);
			}
		}
	}


}




//////////////////////////////////////////////////////////// SLIDEBAR //////////////////////////////////////////////////////////



SlideBar::SlideBar()
{
	/* Constructeur de l'objet */
}


SlideBar::SlideBar(std::string id, float minValue, float maxValue)
{
	/* Constructeur de l'objet */

	m_id = id;
	m_overall = false;
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_state = InterfaceObject::NONE;
	if(m_maxValue <= m_minValue)
		m_maxValue = m_minValue + 1;

	m_currentValue = minValue;
	m_ratio_cursor_bar=-1;
}


SlideBar::~SlideBar()
{
	/* Destructeur de l'objet */

}




// Getters

float SlideBar::getMinValue() const
{
	/* Return the minimum value allowed by the slidebar*/

	return m_minValue;
}

float SlideBar::getMaxValue() const
{
	/* Return the maximum value allowed by the slidebar*/

	return m_maxValue;
}

float SlideBar::getCurrentValue() const
{
	/* Return the current value of the slidebar*/

	return m_currentValue;
}

SlideBar::Type SlideBar::getType() const
{
	/* Return the current value of the slidebar*/

	return m_type;
}


//Setters

void SlideBar::setMinValue(float minValue)
{
	/* Set the minimum value allowed by the slidebar*/

	if(m_maxValue > minValue)	
		m_minValue = minValue;
	else
		m_minValue = m_maxValue - 1;
}

void SlideBar::setMaxValue(float maxValue)
{
	/* Set the maximum value allowed by the slidebar*/

	if(maxValue > m_minValue)
		m_maxValue = maxValue;
	else
		m_maxValue = m_minValue + 1;
}

void SlideBar::setCurrentValue(float currentValue)
{	
	/* Set the current value of the slidebar*/

	m_currentValue = currentValue;
}

void SlideBar::setCurrentValueFromPos(sf::Vector2f clickPos)
{
	/* Set the current value of the slidebar depending on the click position*/

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

void SlideBar::setRatioCursorBar(float ratio_cursor_bar)
{
	m_ratio_cursor_bar = ratio_cursor_bar;
}


void SlideBar::setType(Type type)
{
	/* Set the type of the slidebar (HORIZONTAL or VERTICAL) */

	m_type = type;
}



//Méthodes
void SlideBar::interactsWithUser(sf::RenderWindow* window)
{
	/* Allows the user to interact with the slidebar by clicking on it and change its value */

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect((sf::Vector2i)this->getPosition(), (sf::Vector2i)this->getSize()).contains(mousePos.x, mousePos.y))
		m_state = InterfaceObject::HOVER;
	else if(m_state == InterfaceObject::HOVER && sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		m_state = InterfaceObject::CLICK;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = InterfaceObject::NONE;

	if(m_state == InterfaceObject::CLICK)
	{
		this->setCurrentValueFromPos(mousePos);
	}

	this->update();
}

void SlideBar::update()
{
	/* Updates the slidebar and check if the values are coherent */

	if(m_currentValue > m_maxValue)	
		m_currentValue = m_maxValue;

	if(m_currentValue < m_minValue)	
		m_currentValue = m_minValue;

}


void SlideBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the slidebar */

	// Draw the background of the slidebar
	sf::RectangleShape zone_bar;
	zone_bar.setFillColor(sf::Color(50,50,50,180));
	zone_bar.setOutlineThickness(2);
	zone_bar.setOutlineColor(sf::Color(40,40,40,200));
	zone_bar.setSize(m_size);
	zone_bar.setPosition(m_position);
	target.draw(zone_bar, states);



	// Draw the sliding bar
	sf::RectangleShape sliding_bar;
	sliding_bar.setFillColor(sf::Color(150, 150, 150, 225));

	float coeff_size = 1./(m_maxValue*0.01-m_minValue*0.01);
	if(m_size.x * coeff_size > m_size.x) coeff_size = 1;
	if(m_size.x * coeff_size < m_size.x * 1./20.) coeff_size = 1./20.;

	if(m_type==HORIZONTAL)
	{
		if(m_ratio_cursor_bar==-1)
			sliding_bar.setSize(sf::Vector2f(m_size.x * coeff_size, m_size.y));
		else
			sliding_bar.setSize(sf::Vector2f(m_size.x * m_ratio_cursor_bar, m_size.y));

		sliding_bar.setPosition(m_position.x + (m_size.x - sliding_bar.getSize().x)*(m_currentValue - m_minValue)/(m_maxValue - m_minValue), m_position.y);
	}
	else
	{
		if(m_ratio_cursor_bar==-1)
			sliding_bar.setSize(sf::Vector2f(m_size.x, m_size.y * coeff_size));
		else
			sliding_bar.setSize(sf::Vector2f(m_size.x, m_size.y * m_ratio_cursor_bar));

		sliding_bar.setPosition(m_position.x , m_position.y + (m_size.y - sliding_bar.getSize().y)*(m_currentValue - m_minValue)/(m_maxValue - m_minValue));
	}

	target.draw(sliding_bar, states);
}







//////////////////////////////////////////////////////////// ScrollingList //////////////////////////////////////////////////////////



ScrollingList::ScrollingList()
{
	/* Constructeur de l'objet */
}


ScrollingList::ScrollingList(std::string id, int font_height, int unrollList_height, int width)
{
	/* Constructeur de l'objet */
	m_id = id;
	m_state = InterfaceObject::NONE;
	m_overall = true;
	m_default_text = "";
	m_font_height = font_height;
	m_unrollList_height = unrollList_height;
	m_width = width;

	m_nbrValues = 0;
	m_currentValue = "";
	m_scrollingBar.setType(SlideBar::VERTICAL);
	m_scrollingBar.setCurrentValue(0);


	m_unroll = false;
	m_size = sf::Vector2f(m_width, m_font_height * SCROLLINGLIST_HEIGHT_COEFF);


	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	else
	{
		sf::Text tmp_char("ScrollingListInitialisation", m_font, m_font_height);		
	}
}

ScrollingList::ScrollingList(std::string id, int font_height, int unrollList_height, int width, std::string default_text)
{
	/* Constructeur de l'objet */
	m_id = id;
	m_state = InterfaceObject::NONE;
	m_overall = true;
	m_default_text = default_text;
	m_font_height = font_height;
	m_unrollList_height = unrollList_height;
	m_width = width;

	m_nbrValues = 0;
	m_currentValue = "";
	m_scrollingBar.setType(SlideBar::VERTICAL);
	m_scrollingBar.setCurrentValue(0);


	m_unroll = false;
	m_size = sf::Vector2f(m_width, m_font_height * SCROLLINGLIST_HEIGHT_COEFF);


	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	else
	{
		sf::Text tmp_char("ScrollingListInitialisation", m_font, m_font_height);		
	}
}


ScrollingList::~ScrollingList()
{
	/* Destructeur de l'objet */

}




// Getters


sf::Rect<float> ScrollingList::getInteractableZone()
{
	sf::Vector2f tmpsize;

	if(!m_unroll)
		tmpsize = m_size;
	else
	{
		if(m_unrollList_height >= (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF) * m_nbrValues)
			tmpsize = sf::Vector2f(m_width, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF) * (m_nbrValues+1));
		else
			tmpsize = sf::Vector2f(m_width, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF) + m_unrollList_height);
	}

	return sf::Rect<float>(m_position, tmpsize);

}


int ScrollingList::getWidth()
{
	return m_width;
}


int ScrollingList::getFontHeight()
{
	return m_font_height;
}


std::string ScrollingList::getCurrentValue()
{
	/* Return the current value of the slidebar*/

	return m_currentValue;
}


int ScrollingList::getNbrValues()
{
	return m_nbrValues;
}


bool ScrollingList::isUnroll()
{
	return m_unroll;
}






//Setters


void ScrollingList::setWidth(int width)
{
	m_width = width;
}


void ScrollingList::setFontHeight(int font_height)
{
	m_font_height = font_height;
}


void ScrollingList::setCurrentValue(std::string currentValue)
{	
	/* Set the current value of the slidebar*/

	m_currentValue = currentValue;
}


//Methods

void ScrollingList::addValue(std::string value)
{
	m_values.push_back(ScrollingList::Option{value, ScrollingList::NONE});
	m_nbrValues++;
	m_texture.create(m_width, m_nbrValues * m_font_height * SCROLLINGLIST_HEIGHT_COEFF);

	m_scrollingBar.setMinValue(0);
	m_scrollingBar.setMaxValue(m_nbrValues * m_font_height * SCROLLINGLIST_HEIGHT_COEFF - m_unrollList_height);
	m_scrollingBar.update();
}


void ScrollingList::clearList()
{
	/* Function to clear list of values */

	m_values.clear();
}



void ScrollingList::interactsWithUser(sf::RenderWindow* window)
{
	/* Function to interact with the user*/
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Rect<float>(this->getPosition(), sf::Vector2f(m_width, m_font_height * SCROLLINGLIST_HEIGHT_COEFF)).contains(mousePos))
		m_state = InterfaceObject::HOVER;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = InterfaceObject::NONE;
	else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_state == InterfaceObject::HOVER)
	{
		m_state = InterfaceObject::CLICK;
		m_unroll^=true;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->getInteractableZone().contains(mousePos))
		m_unroll=false;

	//if(m_state!=CLICK)
	//	m_isClick = false;

	if(m_unroll)
	{
		sf::Rect<float> options_zone;
		if(m_unrollList_height < m_font_height * SCROLLINGLIST_HEIGHT_COEFF * m_nbrValues)
		{
			options_zone = sf::Rect<float>(this->getPosition()+sf::Vector2f(0, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF)), sf::Vector2f(m_width - m_scrollingBar.getSize().x, m_unrollList_height));
			m_scrollingBar.interactsWithUser(window);
		}
		else
			options_zone = sf::Rect<float>(this->getPosition()+sf::Vector2f(0, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF)), sf::Vector2f(m_width, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF) * m_nbrValues));

		
		int i = m_position.y + (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF);
		for(std::vector<struct Option>::iterator it=m_values.begin(); it != m_values.end(); it++)
		{
			if(options_zone.contains(mousePos))
			{
				if(mousePos.y + m_scrollingBar.getCurrentValue() >= i && mousePos.y + m_scrollingBar.getCurrentValue() < i +  (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF))
				{
					if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						if(it->state == ScrollingList::CLICK)
						{
							m_currentValue = it->value;
							m_unroll = false;
						}

						it->state = ScrollingList::HOVER;
					}
					else if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
						it->state = ScrollingList::CLICK;
				}
				else
				{
					it->state = ScrollingList::NONE;
				}
				
				i += (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF);
			}
			else
				it->state = ScrollingList::NONE;
		}
		
	}
}


void ScrollingList::update()
{
	m_size = sf::Vector2f(m_width,  (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF));

	m_texture.clear(sf::Color(0,0,0,0));

	int pos_value_bar=0;
	for(std::vector<struct Option>::iterator it = m_values.begin(); it != m_values.end(); it++)
	{
		sf::Text value_text(it->value, m_font, m_font_height);
		sf::RectangleShape value_bar(sf::Vector2f(m_width,  (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF)));
		
		if(it->state==ScrollingList::CLICK)
			value_bar.setFillColor(sf::Color(80, 80, 80, 225));
		else if(it->state==ScrollingList::HOVER)
			value_bar.setFillColor(sf::Color(110, 110, 110, 225));
		else
			value_bar.setFillColor(sf::Color(150, 150, 150, 225));


		value_text.setOrigin(value_text.getLocalBounds().left, value_text.getLocalBounds().top);
		value_bar.setPosition(sf::Vector2f(0, pos_value_bar));
		value_text.setPosition((int)(value_bar.getPosition().x + value_bar.getSize().x/2 - value_text.getLocalBounds().width/2), (int)(value_bar.getPosition().y + value_bar.getSize().y/2 - value_text.getLocalBounds().height/2));

		m_texture.draw(value_bar);
		m_texture.draw(value_text);
		pos_value_bar += value_bar.getSize().y;
	}

	m_texture.display();

	m_scrollingBar.setSize(sf::Vector2f(20, m_unrollList_height));
	m_scrollingBar.setPosition(sf::Vector2f(m_position.x + m_width - m_scrollingBar.getSize().x, m_position.y +  (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF)));
	m_scrollingBar.setRatioCursorBar((float)m_unrollList_height/(float)(m_texture.getTexture().getSize().y));
	m_scrollingBar.update();
}



void ScrollingList::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Text value_text;
	value_text.setFont(m_font);
	value_text.setCharacterSize(m_font_height);

	if(strcmp(m_currentValue.c_str(), "")==0)
	{
		value_text.setString(m_default_text);
		value_text.setFillColor(sf::Color(180,180,180,225));
	} 	
	else
		value_text.setString(m_currentValue);

	sf::RectangleShape value_bar(sf::Vector2f(m_width, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF)));
	
	value_bar.setOutlineThickness(1);
	value_bar.setOutlineColor(sf::Color(245, 245, 245, 245));
	value_bar.setFillColor(sf::Color(110, 110, 110, 200));
	value_bar.setPosition(m_position);

	value_text.setOrigin(value_text.getLocalBounds().left, value_text.getLocalBounds().top);
	value_text.setPosition((int)(value_bar.getPosition().x + value_bar.getSize().x/2 - value_text.getLocalBounds().width/2), (int)(value_bar.getPosition().y + value_bar.getSize().y/2 - value_text.getLocalBounds().height/2));


	if(m_unroll)
	{
		value_bar.setFillColor(sf::Color(150, 150, 150, 200));

		sf::Sprite list_to_draw_sprite(m_texture.getTexture());
		list_to_draw_sprite.setPosition(sf::Vector2f(m_position.x, m_position.y + value_bar.getSize().y));

		if(m_unrollList_height >= (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF) * m_nbrValues)
		{
			list_to_draw_sprite.setTextureRect(sf::IntRect(0, m_scrollingBar.getCurrentValue(), m_width, (int)(m_font_height * SCROLLINGLIST_HEIGHT_COEFF) * m_nbrValues));
			target.draw(list_to_draw_sprite, states);
		}
		else
		{
			list_to_draw_sprite.setTextureRect(sf::IntRect(0, m_scrollingBar.getCurrentValue(), m_width, m_unrollList_height));
			target.draw(list_to_draw_sprite, states);
			target.draw(m_scrollingBar, states);
		}
	}


	target.draw(value_bar, states);
	target.draw(value_text, states);
}









//////////////////////////////////////////////////////////// TextZone //////////////////////////////////////////////////////////


//Constructors/Destructors

TextZone::TextZone()
{

}


TextZone::~TextZone()
{

}


TextZone::TextZone(std::string id, int font_size, std::string text, bool background)
{
	m_id = id;
	m_state = InterfaceObject::NONE;
	m_overall = false;
	m_color = sf::Color(255,255,255,255);
	m_font_size = font_size;
	m_text_string = text;
	m_background = background;


	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	
	m_text.setFont(m_font);
	m_text.setString(m_text_string);
	m_text.setCharacterSize(m_font_size);
	m_text.setOrigin(m_text.getLocalBounds().left, m_text.getLocalBounds().top);
	m_size = sf::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height);
}



//Getters

int TextZone::getFontSize()
{
	return m_font_size;
}


std::string TextZone::getText()
{
	return m_text_string;
}


bool TextZone::hasBackground()
{
	return m_background;
}


sf::Color TextZone::getColor()
{
	return m_color;
}



//Setters



void TextZone::setFontSize(int font_size)
{
	m_font_size = font_size;
}


void TextZone::setText(std::string text)
{
	m_text_string = text;
}


void TextZone::setBackground(bool background)
{
	m_background = background;
}


void TextZone::setColor(sf::Color color)
{
	m_color = color;
}


//Methods

void TextZone::setSizeToText()
{
	m_size = sf::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height);
}

void TextZone::update()
{
	m_text.setPosition(m_position + sf::Vector2f(5,5));
	m_text.setOrigin(sf::Vector2f(m_text.getLocalBounds().left, m_text.getLocalBounds().top));
	m_text.setString(m_text_string);
	m_text.setCharacterSize(m_font_size);
	m_text.setFillColor(m_color);
}


void TextZone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_background)
	{
		sf::RectangleShape rect_background(m_size + sf::Vector2f(2,2));
		rect_background.setPosition(m_position);
		rect_background.setFillColor(sf::Color(180,180,180,200));
		target.draw(rect_background);
		rect_background.setSize(m_size);
		rect_background.setFillColor(sf::Color(100,100,100,200));
		target.draw(rect_background);
	}
	
	target.draw(m_text, states);

}











//////////////////////////////////////////////////////////// EntityDisplayer //////////////////////////////////////////////////////////



//Constructors/Destructors

EntityDisplayer::EntityDisplayer()
{

}


EntityDisplayer::~EntityDisplayer()
{
	if(m_sprite != NULL)
		delete m_sprite;
}


EntityDisplayer::EntityDisplayer(std::string id, bool background)
{
	m_id = id;
	m_state = InterfaceObject::NONE;
	m_overall = false;
	m_background = background;

	m_entity = NULL;
	m_sprite = NULL;
}


//Getters

bool EntityDisplayer::hasBackground()
{
	return m_background;
}

Entity* EntityDisplayer::getEntity()
{
	return m_entity;
}



//Setters

void EntityDisplayer::setBackground(bool background)
{	
	m_background = background;
}


void EntityDisplayer::setEntity(Entity* entity)
{
	m_entity = entity;

	if(m_sprite != NULL)
	{
		delete m_sprite;
		m_sprite = NULL;
	}

	if(m_entity!=NULL)
	{
		
		if(m_entity->isAffiliatedToMap())
			m_sprite = new AnimatedSpriteInMap();
		else
			m_sprite = new AnimatedSprite();
		
		*m_sprite = *(m_entity->getSprite());
	}
}


//Methods

void EntityDisplayer::update()
{
	if(m_sprite!=NULL)
	{
		m_sprite->setPosition(sf::Vector2f((2*m_position.x + m_size.x)/2 - m_sprite->getSize().x/2 , (2*m_position.y + m_size.y)/2 - m_sprite->getSize().y/2));
		m_sprite->update();
	}
}


void EntityDisplayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_background)
	{
		sf::RectangleShape rect_background(m_size + sf::Vector2f(2,2));
		rect_background.setPosition(m_position);
		rect_background.setFillColor(sf::Color(180,180,180,200));
		target.draw(rect_background);
		rect_background.setSize(m_size);
		rect_background.setFillColor(sf::Color(100,100,100,200));
		target.draw(rect_background);

		sf::VertexArray grid(sf::Lines);
		grid.resize(4);
		grid[0].position = sf::Vector2f(m_position.x + m_size.x/2, m_position.y); grid[1].position = sf::Vector2f(m_position.x + m_size.x/2, m_position.y + m_size.y);
		grid[2].position = sf::Vector2f(m_position.x, m_position.y + m_size.y/2); grid[3].position = sf::Vector2f(m_position.x + m_size.x, m_position.y + m_size.y/2);
		grid[0].color = sf::Color(200,200,200,200);	grid[1].color = sf::Color(200,200,200,200);	grid[2].color = sf::Color(200,200,200,200);	grid[3].color = sf::Color(200,200,200,200);
		target.draw(grid);
		
	}

	if(m_sprite!=NULL)
	{
		target.draw(*m_sprite, states);
	}
}

