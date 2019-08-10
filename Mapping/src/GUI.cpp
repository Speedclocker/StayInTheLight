#include "GUI.h"
#include "Map.h"
#include "tools.h"
#include "Character.h"
#include "Collector.h"
#include "ResourcesManagement.h"


#define CHARACTER_SIZE_HEIGHT 20
#define FONT_FILE "../data/fonts/AldoTheApache.ttf"

#define FULLSCREEN true


MappingGUI::MappingGUI()
{
	/* Constructor */

	m_window = NULL;
	m_windows_manager = NULL;
	m_resources_manager = NULL;
	m_map = NULL;
	m_available_tiles = NULL;
	m_tile_size = 0;
	m_tileset_texture = NULL;
	m_ptr_event_txt_entered = NULL;
	m_chosen_height = 0;
	m_chosen_tile = {sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	m_chosen_entity = NULL;
	m_transparency_map = false;
	m_available_tiles = NULL;
	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "An error occured while loading the font from a file, name of font may be incorrect." << std::endl;

	// Setting Tile
	multiple_setting_tile_click = false;
	set_tile_origin_click = sf::Vector2f(0,0);
	set_tile_corner_click = sf::Vector2f(0,0);

	// Moving View
	move_view_click = false;

	// Setting Height
	keyPressed_lower_layer = false;
	keyPressed_higher_layer = false; 
	keyPressed_transparency = false;
}


MappingGUI::~MappingGUI()
{
	/* Destructor */

	if(m_windows_manager != NULL)
		delete m_windows_manager;

	if(m_chosen_entity != NULL)
		delete m_chosen_entity;

	if(m_available_tiles != NULL)
		free(m_available_tiles);
}


MappingGUI::MappingGUI(sf::RenderWindow* window)
{
	/* Constructor */

	m_window = window;
	m_windows_manager = new BWManager(window);
	m_resources_manager = NULL;
	m_map = NULL;
	m_tileset_texture_file_name = "";
	m_available_tiles = NULL;
	m_tile_size = 0;
	m_tileset_texture = NULL;
	m_ptr_event_txt_entered = NULL;
	m_chosen_height = 0;
	m_chosen_tile = {sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	m_chosen_entity = NULL;
	m_chosen_entity_file_name = "";
	m_transparency_map = false;
	m_nbr_available_tiles = -1;
	m_available_tiles = NULL;
	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "An error occured while loading the font from a file, name of font may be incorrect." << std::endl;

	// Setting Tile
	multiple_setting_tile_click = false;
	set_tile_origin_click = sf::Vector2f(0,0);
	set_tile_corner_click = sf::Vector2f(0,0);

	// Moving View
	move_view_click = false;

	// Setting Height
	keyPressed_lower_layer = false;
	keyPressed_higher_layer = false; 
	keyPressed_transparency = false;
}



MappingGUI::MappingGUI(sf::RenderWindow* window, Map** map, std::vector<Entity*>* entities, ResourcesManager* resources_manager, std::string tileset_texture_file_name)
{
	/* Constructor */

	m_window = window;
	m_windows_manager = new BWManager(window);
	m_tileset_texture = NULL;
	m_resources_manager = resources_manager;
	m_map = map;
	m_tileset_texture_file_name = tileset_texture_file_name; 
	m_entities = entities;
	m_available_tiles = NULL;
	m_ptr_event_txt_entered = NULL;
	m_chosen_height = 0;
	m_chosen_tile = {sf::Vector2f(-1, -1), sf::Vector2f(-1, -1), false};
	m_chosen_entity = NULL;
	m_chosen_entity_file_name = "";
	m_transparency_map = false;
	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "An error occured while loading the font from a file, name of font may be incorrect." << std::endl;

	if(m_resources_manager != NULL && (m_tileset_texture = m_resources_manager->getOrAddTexture(m_tileset_texture_file_name)) == NULL) throw std::string("Texture file cannot be load.");


	if(setTilesetTexture(m_tileset_texture) < 0) throw "An error occured while setting texture for tileset.";

	if((*m_map) != NULL && m_tileset_texture != NULL && m_windows_manager != NULL)
	{
		(*m_map)->setTexture(m_tileset_texture);
		m_tile_size = (*m_map)->getTileSize();
		this->initializeBoxWindows();

	}


	// Setting Tile
	multiple_setting_tile_click = false;
	set_tile_origin_click = sf::Vector2f(0,0);
	set_tile_corner_click = sf::Vector2f(0,0);

	// Moving View
	move_view_click = false;

	// Setting Height
	keyPressed_lower_layer = false;
	keyPressed_higher_layer = false; 
	keyPressed_transparency = false;
}


// Getters

sf::RenderWindow* MappingGUI::getWindow(void)
{
	return m_window;
}


BWManager* MappingGUI::getBWManager(void)
{
	return m_windows_manager;
}


std::string* MappingGUI::getStringPtrToTextEvent(void)
{
	return m_ptr_event_txt_entered;
}






// Setters

void MappingGUI::setWindow(sf::RenderWindow* window)
{
	m_window = window;
	if(m_windows_manager != NULL)
		m_windows_manager->setWindow(window);
}


int MappingGUI::setTilesetTexture(sf::Texture* tileset_texture)
{
	/* Set a texture for the tileset and reallocating the available tiles */

	m_tileset_texture = tileset_texture;
	if(m_tileset_texture != NULL && (*m_map) != NULL)
	{
		m_tile_size = (*m_map)->getTileSize();

		if(m_available_tiles!=NULL)
			free(m_available_tiles);

		m_nbr_available_tiles = (m_tileset_texture->getSize().x/m_tile_size)*(m_tileset_texture->getSize().y/m_tile_size);

		m_available_tiles = (Tile*)malloc(m_nbr_available_tiles * sizeof(Tile));

		if(m_available_tiles == NULL) { std::cerr << "An error occured while allocating memory for tileset." << std::endl; m_nbr_available_tiles = -1; return -1; }

		for(int i=0; i < m_nbr_available_tiles; i++)
		{
			m_available_tiles[i].m_pos_text = sf::Vector2f( (i%(int)(m_tileset_texture->getSize().x/m_tile_size))*m_tile_size,  (i/(int)(m_tileset_texture->getSize().x/m_tile_size))*m_tile_size );
			m_available_tiles[i].m_size_text = sf::Vector2f( m_tile_size, m_tile_size );
			m_available_tiles[i].m_collisionable = false;
		}
		m_chosen_tile = m_available_tiles[0];

		(*m_map)->setTexture(m_tileset_texture);

		return 1;
	}

	return 0;
}


int MappingGUI::setMap(Map** map)
{
	m_map = map;

	if(m_tileset_texture != NULL && (*m_map) != NULL)
	{
		m_tile_size = (*m_map)->getTileSize();
		free(m_available_tiles);

		m_nbr_available_tiles = (m_tileset_texture->getSize().x/m_tile_size)*(m_tileset_texture->getSize().y/m_tile_size);

		m_available_tiles = (Tile*)malloc(m_nbr_available_tiles * sizeof(Tile));

		if(m_available_tiles == NULL) { std::cerr << "An error occured while allocating memory for tileset." << std::endl; m_nbr_available_tiles = -1; return -1; }

		for(int i=0; i < m_nbr_available_tiles; i++)
		{
			m_available_tiles[i].m_pos_text = sf::Vector2f( (i%(int)(m_tileset_texture->getSize().x/m_tile_size))*m_tile_size,  (i/(int)(m_tileset_texture->getSize().x/m_tile_size))*m_tile_size );
			m_available_tiles[i].m_size_text = sf::Vector2f( m_tile_size, m_tile_size );
			m_available_tiles[i].m_collisionable = false;
		}
		m_chosen_tile = m_available_tiles[0];

		(*m_map)->setTexture(m_tileset_texture);

		return 1;
	}
	return 0;
}




// Methods

void MappingGUI::setTile()
{
	/* Function to set tile in a map */

	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

	if(mousePos.x >=0 && mousePos.x < (*m_map)->getSize().x*(*m_map)->getTileSize()
	&& mousePos.y >=0 && mousePos.y < (*m_map)->getSize().y*(*m_map)->getTileSize() )
	{

		sf::Vector2f positionTile((int)(mousePos.x/(*m_map)->getTileSize())*(*m_map)->getTileSize(), (int)(mousePos.y/(*m_map)->getTileSize())*(*m_map)->getTileSize());

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && m_window->hasFocus())
		{
			// Pose de plusieurs blocs
			if(!multiple_setting_tile_click)
			{
				multiple_setting_tile_click=true;
			}
			
			set_tile_corner_click = positionTile;

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for(int i=set_tile_origin_click.x; (set_tile_origin_click.x<=set_tile_corner_click.x)? i <= set_tile_corner_click.x : i >= set_tile_corner_click.x; (set_tile_origin_click.x<=set_tile_corner_click.x)? i+=m_chosen_tile.m_size_text.x : i-=m_chosen_tile.m_size_text.x)
				{
					for(int j=set_tile_origin_click.y; (set_tile_origin_click.y<=set_tile_corner_click.y)? j <= set_tile_corner_click.y : j >= set_tile_corner_click.y; (set_tile_origin_click.y<=set_tile_corner_click.y)? j+=m_chosen_tile.m_size_text.y : j-=m_chosen_tile.m_size_text.y)
					{	
						(*m_map)->setTile(m_chosen_tile, sf::Vector2i(i, j)/(*m_map)->getTileSize(), m_chosen_height);
					}
				}

				set_tile_origin_click = positionTile;
			}
		}
		else 
		{
			multiple_setting_tile_click=false;

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_window->hasFocus())
			{
				set_tile_origin_click = positionTile;
				(*m_map)->setTile(m_chosen_tile, (sf::Vector2i)positionTile/(*m_map)->getTileSize(), m_chosen_height);
			}
		}
	}
}





void MappingGUI::setEntity()
{
	/* Function to set an entity in map */

	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

	if(mousePos.x >=0 && mousePos.x < (*m_map)->getSize().x*(*m_map)->getTileSize()
	&& mousePos.y >=0 && mousePos.y < (*m_map)->getSize().y*(*m_map)->getTileSize() && m_chosen_entity!=NULL && m_window->hasFocus())
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			set_entity_click = true;
		else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && set_entity_click)
		{
			int index = 1;
			bool name_not_taken = true;
			std::string name; // = m_chosen_entity->getModelName();
			std::string index_str;

			do
			{
				name_not_taken = true;
				index_str = ((index < 10)?"0":"") + std::to_string(index);
				name = m_chosen_entity->getModelName() + "_" + index_str;

				for(std::vector<Entity*>::iterator it = m_entities->begin(); it != m_entities->end() && name_not_taken; it++)
				{
					if(strcmp((*it)->getID().c_str(),name.c_str())==0)
						name_not_taken = false;
				}

				index++;

			} while(!name_not_taken);

			Entity* entity_to_set = NULL;

			if(loadEntity(name, m_chosen_entity_file_name, m_resources_manager, &entity_to_set, *m_map) >= 0)
			{
				entity_to_set->setPosition(sf::Vector2f((int)(mousePos.x - entity_to_set->getSize().x/2), (int)(mousePos.y - entity_to_set->getSize().y/2)));
				entity_to_set->setHeight(m_chosen_height);
				m_entities->push_back(entity_to_set);
				(*m_map)->addEntity(entity_to_set);
			}

/*	
			unsigned int tmp_size = m_entities->size();

			try
			{
				if(strcmp(m_chosen_entity->getType().c_str(), "Character")==0)
					m_entities->push_back(new Character(name, m_chosen_entity_file_name, m_resources_manager, (*m_map)));

				else if(strcmp(m_chosen_entity->getType().c_str(), "Collector")==0)
					m_entities->push_back(new Collector(name, m_chosen_entity_file_name, m_resources_manager, (*m_map)));
			}
			catch(const std::string & e)
			{
				// If an error occured while loading the entity
				if(m_entities->size()>=1 && (*m_entities)[m_entities->size()-1]!=NULL)
					delete (*m_entities)[m_entities->size()-1];
				
				if(tmp_size != m_entities->size())
					m_entities->erase(m_entities->begin() + m_entities->size()-1);

			}
		

			if(tmp_size != m_entities->size())
			{
				(*m_entities)[m_entities->size()-1]->setPosition(sf::Vector2f((int)(mousePos.x - (*m_entities)[m_entities->size()-1]->getSize().x/2), (int)(mousePos.y - (*m_entities)[m_entities->size()-1]->getSize().y/2)));
				(*m_entities)[m_entities->size()-1]->setHeight(m_chosen_height);
				(*m_map)->addEntity((*m_entities)[m_entities->size()-1]);
			}
*/

			set_entity_click = false;
		}
		else
			set_entity_click = false;
	}
	else
		set_entity_click = false;
	
}


		

void MappingGUI::initializeBoxWindows()	
{
	BoxWindow* tmp_boxwindow;
	// -- Initialization Edition Window --

	m_windows_manager->newBoxWindow("Edition", sf::Vector2f(200, 300));
	tmp_boxwindow = m_windows_manager->getBoxWindow("Edition");
	tmp_boxwindow->setMinSize(sf::Vector2f(200, 400));
	tmp_boxwindow->setMaxSize(sf::Vector2f(m_tileset_texture->getSize().x + 10, 800));
	tmp_boxwindow->setPositionWindow(sf::Vector2f(m_windows_manager->getWindow()->getView().getSize().x - tmp_boxwindow->getSize().x - 100, 500));

		// Initialization tab Tileset

	ArgTilesetTab arg_tileset_tab = ArgTilesetTab{m_tileset_texture, &m_available_tiles, &m_nbr_available_tiles, &m_tile_size, &m_chosen_tile, &m_ptr_event_txt_entered};
	tmp_boxwindow->newTab("Tileset", TilesetTab, (ArgTab*)&arg_tileset_tab, sizeof(ArgTilesetTab));

		// Initialization tab Entities

	list_entities(&entities_file_name_list);
	ArgEntitiesTab arg_entities_tab = ArgEntitiesTab{&entities_file_name_list, &m_chosen_entity, &m_chosen_entity_file_name, (*m_map), m_resources_manager};
	tmp_boxwindow->newTab("Entities", EntitiesTab, (ArgTab*)&arg_entities_tab, sizeof(ArgEntitiesTab));

	tmp_boxwindow->update();


	// -- Initialization Info Window --

	m_windows_manager->newBoxWindow("Info", sf::Vector2f(200, 300));
	tmp_boxwindow = m_windows_manager->getBoxWindow("Info");
	tmp_boxwindow->setPositionWindow(sf::Vector2f(m_windows_manager->getWindow()->getView().getSize().x - tmp_boxwindow->getSize().x - 300, 100));
	tmp_boxwindow->setMinSize(sf::Vector2f(200, 200));
	tmp_boxwindow->setMaxSize(sf::Vector2f(200, 200));

		// Initialization tab Info

	ArgInfoTab arg_info_tab = ArgInfoTab{m_map, m_entities};
	tmp_boxwindow->newTab("Info", InfoTab, (ArgTab*)&arg_info_tab, sizeof(ArgInfoTab));

	tmp_boxwindow->update();
}




void MappingGUI::controlView() 
{
	/* Function allowing the movement of window view */
	
	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

	sf::Vector2f refPos = move_view_origin_click;

	sf::View view = m_window->getView();

	if (!move_view_click && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		move_view_origin_click = mousePos;
		view.setCenter(((*m_map)->getSize().x) * (*m_map)->getTileSize()/2, ((*m_map)->getSize().y) * (*m_map)->getTileSize()/2);
	}
	else if(!move_view_click && sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		move_view_origin_click = mousePos;
		move_view_click=true;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		refPos = sf::Vector2f((view.getCenter().x + move_view_origin_click.x - mousePos.x > 0 && view.getCenter().x + move_view_origin_click.x - mousePos.x > 0)? mousePos.x : refPos.x,
							  (view.getCenter().y + move_view_origin_click.y - mousePos.y > 0 && view.getCenter().y + move_view_origin_click.y - mousePos.y > 0)? mousePos.y : refPos.y);

		view.move( ( (view.getCenter().x + move_view_origin_click.x - refPos.x > 0) && (view.getCenter().x + move_view_origin_click.x - refPos.x < (*m_map)->getSize().x * (*m_map)->getTileSize()) )  ? move_view_origin_click.x - refPos.x : 0,
					( (view.getCenter().y + move_view_origin_click.y - refPos.y > 0) && (view.getCenter().y + move_view_origin_click.y - refPos.y < (*m_map)->getSize().y * (*m_map)->getTileSize()) )  ? move_view_origin_click.y - refPos.y : 0);
	}
	else
		move_view_click=false;

	m_window->setView(view);
}








void MappingGUI::heightLevelCommand()
{	
	/* Function allowing to change the current height */

	// Lower Height -
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::O) && m_window->hasFocus())
		keyPressed_lower_layer = true;
	else if(keyPressed_lower_layer)
	{
		keyPressed_lower_layer = false;
		if(m_chosen_height>0) (m_chosen_height)--;
	}

	// Higher Height +
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && m_window->hasFocus())
		keyPressed_higher_layer = true;
	else if(keyPressed_higher_layer)
	{
		keyPressed_higher_layer = false;
		if(m_chosen_height < (*m_map)->getHeight()-1 ) (m_chosen_height)++;
	}

	// Transparency mode
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T) && m_window->hasFocus())
		keyPressed_transparency = true;
	else if(keyPressed_transparency)
	{
		keyPressed_transparency = false;
		m_transparency_map^=true;
	}
}




void MappingGUI::interactWithUser()
{
	if(m_window->hasFocus())
	{
		saveMapCommand((*m_map), *m_entities, m_tileset_texture_file_name, m_available_tiles, m_nbr_available_tiles);
		if(loadMapCommand(m_window, m_map, m_entities, &m_tileset_texture, &m_available_tiles, &m_nbr_available_tiles, m_resources_manager, FULLSCREEN) > 0)
			this->setTilesetTexture(m_tileset_texture);

		this->heightLevelCommand(); // To change height level
		this->controlView();		// To control the view
		if(m_windows_manager != NULL)
		{
			if(m_windows_manager->getFocusBoxWindow()==NULL)
			{
				if( m_windows_manager->getBoxWindow("Edition")!=NULL && m_windows_manager->getBoxWindow("Edition")->getFocusTab()!=NULL && strcmp(m_windows_manager->getBoxWindow("Edition")->getFocusTab()->getTitle().c_str(), "Tileset")==0 )
					this->setTile(); // To set a tile in the map
				if( m_windows_manager->getBoxWindow("Edition")!=NULL && m_windows_manager->getBoxWindow("Edition")->getFocusTab()!=NULL && strcmp(m_windows_manager->getBoxWindow("Edition")->getFocusTab()->getTitle().c_str(), "Entities")==0 )
					this->setEntity(); // To set an entity in the map
			} 
			m_windows_manager->interactionsManagement(); // To catch interactions between user and the internal box windows
		}
	} 
}


void MappingGUI::update()
{
	if((*m_map) != NULL)
		m_tile_size = (*m_map)->getTileSize();


	if(m_transparency_map)
		(*m_map)->update_transparency(m_chosen_height);
	else
		(*m_map)->update();


	if(m_chosen_entity != NULL)
	{	
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
		m_chosen_entity->setPosition(sf::Vector2f((int)(mousePos.x - m_chosen_entity->getSize().x/2), int(mousePos.y - m_chosen_entity->getSize().y/2)));
		m_chosen_entity->update();
	}

	for(std::vector<Entity*>::iterator it = m_entities->begin(); it != m_entities->end(); it++)
	{
		(*it)->update();
	}


	if(m_windows_manager != NULL)
		m_windows_manager->update();

}


void MappingGUI::draw()
{
	//-------------------------------
	// Map and Rectangle Zone Map
	if((*m_map) != NULL)
	{
		// Zone Map
		sf::RectangleShape map_zone(sf::Vector2f((*m_map)->getSize().x*(*m_map)->getTileSize(), (*m_map)->getSize().y*(*m_map)->getTileSize()));
		map_zone.setFillColor(sf::Color(0,0,0,0));
		map_zone.setOutlineThickness(3);
		map_zone.setOutlineColor(sf::Color::Red);
		m_window->draw(map_zone);

		// Map
		m_window->draw(**m_map);
	}


	//-------------------------------
	// Current Height
	char height_str[40];

	if(m_transparency_map)
		sprintf(height_str, "Current Height : %d T-Mode", m_chosen_height);
	else
		sprintf(height_str, "Current Height : %d", m_chosen_height);

	sf::Text height_text(height_str,m_font);
	height_text.setCharacterSize(CHARACTER_SIZE_HEIGHT);
	height_text.setFillColor(sf::Color::White);
	height_text.setOutlineColor(sf::Color::Black);
	height_text.setOutlineThickness(1);
	height_text.setPosition(m_window->getView().getCenter().x - m_window->getView().getSize().x/2 + 10, m_window->getView().getCenter().y + m_window->getView().getSize().y/2 - 30);

	m_window->draw(height_text);



	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
	//-------------------------------
	// Setting Tile
	if( m_windows_manager->getBoxWindow("Edition")!=NULL && m_windows_manager->getBoxWindow("Edition")->getFocusTab()!=NULL && strcmp(m_windows_manager->getBoxWindow("Edition")->getFocusTab()->getTitle().c_str(), "Tileset")==0 )
	{
		if( mousePos.x >=0 && mousePos.x < (*m_map)->getSize().x*(*m_map)->getTileSize()
		&& mousePos.y >=0 && mousePos.y < (*m_map)->getSize().y*(*m_map)->getTileSize() )
		{
			sf::Sprite ghost_tile(*m_tileset_texture, sf::IntRect((sf::Vector2i)m_chosen_tile.m_pos_text, (sf::Vector2i)m_chosen_tile.m_size_text));
			ghost_tile.setColor(sf::Color(255,255,255,100));
			ghost_tile.setPosition((int)(mousePos.x/(*m_map)->getTileSize())*(*m_map)->getTileSize(), (int)(mousePos.y/(*m_map)->getTileSize())*(*m_map)->getTileSize());

			m_window->draw(ghost_tile);
		}
		if(multiple_setting_tile_click)
		{
			sf::Sprite ghost_tile(*m_tileset_texture, sf::IntRect((sf::Vector2i)m_chosen_tile.m_pos_text, (sf::Vector2i)m_chosen_tile.m_size_text));
			ghost_tile.setColor(sf::Color(255,255,255,100));
			ghost_tile.setPosition((int)(mousePos.x/(*m_map)->getTileSize())*(*m_map)->getTileSize(), (int)(mousePos.y/(*m_map)->getTileSize())*(*m_map)->getTileSize());

			for(int i=set_tile_origin_click.x; (set_tile_origin_click.x<=set_tile_corner_click.x)? i <= set_tile_corner_click.x : i >= set_tile_corner_click.x; (set_tile_origin_click.x<=set_tile_corner_click.x)? i+=m_chosen_tile.m_size_text.x : i-=m_chosen_tile.m_size_text.x)
			{
				for(int j=set_tile_origin_click.y; (set_tile_origin_click.y<=set_tile_corner_click.y)? j <= set_tile_corner_click.y : j >= set_tile_corner_click.y; (set_tile_origin_click.y<=set_tile_corner_click.y)? j+=m_chosen_tile.m_size_text.y : j-=m_chosen_tile.m_size_text.y)
				{
					ghost_tile.setPosition(sf::Vector2f(i, j));

					m_window->draw(ghost_tile);
				}
			}
		}
	}



	//-------------------------------
	// Setting Entity
	if( m_windows_manager->getBoxWindow("Edition")!=NULL && m_windows_manager->getBoxWindow("Edition")->getFocusTab()!=NULL && strcmp(m_windows_manager->getBoxWindow("Edition")->getFocusTab()->getTitle().c_str(), "Entities")==0 )
	{
		if( mousePos.x >=0 && mousePos.x < (*m_map)->getSize().x*(*m_map)->getTileSize()
		&& mousePos.y >=0 && mousePos.y < (*m_map)->getSize().y*(*m_map)->getTileSize() )
		{
			if(m_chosen_entity!=NULL)
			{
				sf::Text entity_pos_text(std::to_string((int)m_chosen_entity->getPosition().x) + "," + std::to_string((int)m_chosen_entity->getPosition().y),m_font);
				entity_pos_text.setCharacterSize(10);
				entity_pos_text.setFillColor(sf::Color::White);
				entity_pos_text.setOutlineColor(sf::Color::Black);
				entity_pos_text.setOutlineThickness(1);
				entity_pos_text.setPosition(m_chosen_entity->getPosition().x + m_chosen_entity->getSize().x, m_chosen_entity->getPosition().y + m_chosen_entity->getSize().y);

				m_window->draw(*m_chosen_entity);

				m_window->draw(entity_pos_text);
			}
		}
	}
	

	//--------------------------------
	// BWManager
	if(m_windows_manager != NULL)
		m_windows_manager->drawWindows();

	//--------------------------------
}
