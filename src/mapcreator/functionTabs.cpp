#include "functionTabs.h"
#include "SaveLoadContent.h"

#include "ResourcesManagement.h"

#define FONT_FILE "data/fonts/AldoTheApache.ttf"
#define ENTITIES_FOLDER "data/entities/"
#define BOXWINDOW_SIZE_CHARACTER 16



void TilesetTab(Tab* tab, ArgTab* argtab)
{
	sf::Texture** texture = ((ArgTilesetTab*)(argtab))->texture;
	Tile** tiles = ((ArgTilesetTab*)(argtab))->tiles;
	const int* nbr_avail_tiles= ((ArgTilesetTab*)(argtab))->nbr_avail_tiles;
	const int* size_tile = ((ArgTilesetTab*)(argtab))->size_tile;
	Tile* target_tile = ((ArgTilesetTab*)(argtab))->target_tile;
	ResourcesManager* ptr_resources_manager = (((ArgTilesetTab*)(argtab))->ptr_resources_manager);
	std::string** ptr_text_event = ((ArgTilesetTab*)(argtab))->text_event_location;

	sf::Rect<float> zone_tileset(sf::Vector2f(0, 0), sf::Vector2f(tab->getSize().x - 10, (*texture)->getSize().y )); // Fait dépendre la taille du tileset de la fenêtre


	// Outils interface -- Barre de slide pour le tileset et miniature tileset
	SlideBar* tileset_slidebar;
	TilesetSelect* tile_selector;
	Button* tileset_file_valid_button;
	InputBar* tileset_file_entry;

	if(!tab->initialized)
	{
		//Initialisation du tileset
		tile_selector = new TilesetSelect("tile_selector", *texture, zone_tileset, tiles, nbr_avail_tiles, size_tile, target_tile);
		tile_selector->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));
		tile_selector->setZone(zone_tileset);


		//Initialisation Slidebar1 
		tileset_slidebar = new SlideBar("tileset_slidebar", 0, tile_selector->getMaxZonePos().x);
		tileset_slidebar->setSize(sf::Vector2f(tile_selector->getZone().width, 15));
		tileset_slidebar->setType(SlideBar::HORIZONTAL);
		tileset_slidebar->setPosition(sf::Vector2f(tile_selector->getPosition().x , tile_selector->getPosition().y + tile_selector->getZone().height + 5) );


		//Initialisation Button
		tileset_file_valid_button = new Button("tileset_file_valid_button", Button::ONE_STATE, "Change", BOXWINDOW_SIZE_CHARACTER);
		tileset_file_valid_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x - (tileset_file_valid_button->getSize().x + 5), tileset_slidebar->getPosition().y + tileset_slidebar->getSize().y + 10));


		//Initialisation InputBar
		tileset_file_entry = new InputBar("tileset_file_entry", BOXWINDOW_SIZE_CHARACTER, 100, InputBar::ALPHANUMERICAL_SPEC_CHARACTERS, ptr_text_event, "Texture File");
		tileset_file_entry->setSize(sf::Vector2f(tab->getSize().x - (tileset_file_valid_button->getSize().x + 15), tileset_file_entry->getSize().y));
		tileset_file_entry->setPosition(tileset_slidebar->getPosition() + sf::Vector2f(0, tileset_slidebar->getSize().y + 10));
		


		//Ajout des objets à l'onglet
		tab->addObject(tileset_slidebar);
		tab->addObject(tile_selector);
		tab->addObject(tileset_file_entry);
		tab->addObject(tileset_file_valid_button);

		tab->initialized = true;
	}
	else	
	{
		//Initialization objects
		tile_selector = dynamic_cast<TilesetSelect*>(tab->getObject("tile_selector"));
		tileset_slidebar = dynamic_cast<SlideBar*>(tab->getObject("tileset_slidebar"));
		tileset_file_entry = dynamic_cast<InputBar*>(tab->getObject("tileset_file_entry"));
		tileset_file_valid_button = dynamic_cast<Button*>(tab->getObject("tileset_file_valid_button"));


		//Paramétrage du tileset
		tile_selector->setZone(zone_tileset);
		tile_selector->setZone(sf::Rect<float>(tileset_slidebar->getCurrentValue(), tile_selector->getZone().top, tile_selector->getZone().width, tile_selector->getZone().height));
		tile_selector->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));


		//Paramétrage des slidebar
		tileset_slidebar->setPosition(sf::Vector2f(tile_selector->getPosition().x , tile_selector->getPosition().y + tile_selector->getZone().height + 5) );
		tileset_slidebar->setSize(sf::Vector2f(tile_selector->getZone().width, 15));
		tileset_slidebar->setRatioCursorBar((float)tile_selector->getZone().width/(float)(tile_selector->getMaxZonePos().x + tile_selector->getZone().width));
		tileset_slidebar->setMaxValue(tile_selector->getMaxZonePos().x);


		//Paramétrage du button
		tileset_file_valid_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x - (tileset_file_valid_button->getSize().x + 5), tileset_slidebar->getPosition().y + tileset_slidebar->getSize().y + 10));


		//Paramétrage du InputBar
		tileset_file_entry->setSize(sf::Vector2f(tab->getSize().x - (tileset_file_valid_button->getSize().x + 15), tileset_file_entry->getSize().y));
		tileset_file_entry->setPosition(tileset_slidebar->getPosition() + sf::Vector2f(0, tileset_slidebar->getSize().y + 10));


		//Stocke la valeur du tile cible
		*target_tile = tile_selector->getChosenTile();

		if(tileset_file_valid_button->isClicked())
		{
			sf::Texture* tmp_texture = NULL;
			tmp_texture = ptr_resources_manager->getOrAddTexture(tileset_file_entry->getValue());

			if(tmp_texture != NULL)
				(*texture) = tmp_texture;
		}

		tile_selector->setTexture(*texture);
	}

}









void InfoTab(Tab* tab, ArgTab* argtab)
{
	Map** ptr_map = ((ArgInfoTab*)(argtab))->ptr_map;
	std::vector<Entity*>* entities = ((ArgInfoTab*)(argtab))->entities;

	std::string all_titles="Size \nHeight \nEntities \n";
	std::string size = (((*ptr_map)!=NULL)?std::to_string((int)(*ptr_map)->getSize().x):"X") + " * " + (((*ptr_map)!=NULL)?std::to_string((int)(*ptr_map)->getSize().y):"X") + " \n";
	std::string height = (((*ptr_map)!=NULL)?std::to_string((int)(*ptr_map)->getHeight()):"X") +" \n";
	//std::string texture = "Texture : " + map->getHeight() + " \n";
	std::string nbr_entities = ((entities!=NULL)?std::to_string(entities->size()):"X") +" \n";


	std::string all_values = size + height + nbr_entities;


	if(!tab->initialized)
	{
		// Text zone containing some information about the current map
		TextZone *ptr_textzone_titles, *ptr_textzone_values;

		//Initialization textzone
		ptr_textzone_titles = new TextZone("textzone_titles", BOXWINDOW_SIZE_CHARACTER, all_titles, false);
		ptr_textzone_values = new TextZone("textzone_values", BOXWINDOW_SIZE_CHARACTER, all_values, true);

		ptr_textzone_titles->setSize(sf::Vector2f(100 - 20, tab->getSize().y - (20 + tab->getTitleSize())));
		ptr_textzone_titles->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 5));	
		ptr_textzone_values->setSize(tab->getSize() - sf::Vector2f(100, 20 + tab->getTitleSize()));
		ptr_textzone_values->setPosition(sf::Vector2f(ptr_textzone_titles->getPosition().x + ptr_textzone_titles->getSize().x , ptr_textzone_titles->getPosition().y));	

		// Add object to the tab
		tab->addObject(ptr_textzone_titles);
		tab->addObject(ptr_textzone_values);

		tab->initialized = true;
	}

	else
	{
		//Find the object among tab objects
		TextZone* textzone_titles = dynamic_cast<TextZone*>(tab->getObject("textzone_titles"));
		TextZone* textzone_values = dynamic_cast<TextZone*>(tab->getObject("textzone_values"));

		textzone_titles->setText(all_titles);
		textzone_titles->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 5));

		textzone_values->setText(all_values);
		textzone_values->setPosition(sf::Vector2f(textzone_titles->getPosition().x + textzone_titles->getSize().x , textzone_titles->getPosition().y));	
	}
}







void EntitiesTab(Tab* tab, ArgTab* argtab)
{
	Map* map = (((ArgEntitiesTab*)(argtab))->map);
	Entity** current_entity = (((ArgEntitiesTab*)(argtab))->current_entity);
	std::string* current_entity_file_name = (((ArgEntitiesTab*)(argtab))->current_entity_file_name);
	std::vector<std::string> list_entities = *(((ArgEntitiesTab*)(argtab))->ptr_list_entities);
	ResourcesManager* resources_manager = (((ArgEntitiesTab*)(argtab))->resources_manager);
	bool* set_or_erase_option = (((ArgEntitiesTab*)(argtab))->set_or_erase_option);
	std::string** ptr_text_event = ((ArgEntitiesTab*)(argtab))->text_event_location;


	EntityDisplayer *spritedisplayer = NULL;
	ScrollingList *scrollinglist_entities = NULL;
	InputBar *entity_id_inputbar = NULL;
	Button* set_erase_entity_button = NULL;

	static std::string chosen_entity_string;

	static sf::Texture texture;

	if(!tab->initialized)
	{
		//Initialization EntityDisplayer
		spritedisplayer = new EntityDisplayer("spritedisplayer", true);
		spritedisplayer->setSize(sf::Vector2f(tab->getSize().x - 20, tab->getSize().x - 20));
		spritedisplayer->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 10));



		//Initialization ScrollingList
		scrollinglist_entities = new ScrollingList("scrollinglist_entities", BOXWINDOW_SIZE_CHARACTER, 100, tab->getSize().x - 20, "Choose an entity");
		scrollinglist_entities->setPosition(sf::Vector2f(spritedisplayer->getPosition().x , spritedisplayer->getPosition().y + spritedisplayer->getSize().y + 10) );

		// Adding all values to the ScrollingList
		for(std::vector<std::string>::iterator it=list_entities.begin(); it!=list_entities.end(); it++)
			scrollinglist_entities->addValue(*it);
		


		//Initialization EntityIdInputbar
		entity_id_inputbar = new InputBar("entity_id_inputbar", BOXWINDOW_SIZE_CHARACTER, tab->getSize().x - 20, InputBar::ALPHANUMERICAL, ptr_text_event, "Entity ID");
		entity_id_inputbar->setSize(sf::Vector2f(tab->getSize().x - 20, entity_id_inputbar->getSize().y));
		entity_id_inputbar->setPosition(sf::Vector2f(scrollinglist_entities->getPosition().x , scrollinglist_entities->getPosition().y + scrollinglist_entities->getSize().y + 10));



		//Initialization Set entity Button
		set_erase_entity_button = new Button("set_erase_entity_button", Button::TWO_STATES, "Erase", BOXWINDOW_SIZE_CHARACTER);
		set_erase_entity_button->click();
		set_erase_entity_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - set_erase_entity_button->getSize().x/2, entity_id_inputbar->getPosition().y + entity_id_inputbar->getSize().y + 10));




		// Add object to the tab
		tab->addObject(spritedisplayer);
		tab->addObject(scrollinglist_entities);
		tab->addObject(entity_id_inputbar);
		tab->addObject(set_erase_entity_button);


		chosen_entity_string = scrollinglist_entities->getCurrentValue();

		spritedisplayer->setEntity(*current_entity);

		tab->initialized = true;
	}
	else
	{
		spritedisplayer = dynamic_cast<EntityDisplayer*>(tab->getObject("spritedisplayer"));
		scrollinglist_entities = dynamic_cast<ScrollingList*>(tab->getObject("scrollinglist_entities"));
		entity_id_inputbar = dynamic_cast<InputBar*>(tab->getObject("entity_id_inputbar"));
		set_erase_entity_button = dynamic_cast<Button*>(tab->getObject("set_erase_entity_button"));



		// Set sprite displayer Characteristics
		spritedisplayer->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 10));



		// Set scrolling list Characteristics
		scrollinglist_entities->setPosition(sf::Vector2f(spritedisplayer->getPosition().x , spritedisplayer->getPosition().y + spritedisplayer->getSize().y + 10) );

		if(scrollinglist_entities->getCurrentValue() != chosen_entity_string)
		{
			if(*current_entity != NULL)
			{
				delete *current_entity;
				*current_entity = NULL;
			}

			loadEntity("chosen_entity", std::string(ENTITIES_FOLDER) + scrollinglist_entities->getCurrentValue(), resources_manager, current_entity, map);

			chosen_entity_string = scrollinglist_entities->getCurrentValue();

			if(*current_entity != NULL)
				*current_entity_file_name = std::string(ENTITIES_FOLDER) + scrollinglist_entities->getCurrentValue();
			else
				*current_entity_file_name = "";

			spritedisplayer->setEntity(*current_entity);
		}



		// Set Input Characteristics
		entity_id_inputbar->setSize(sf::Vector2f(tab->getSize().x - 20, entity_id_inputbar->getSize().y));
		entity_id_inputbar->setPosition(sf::Vector2f(scrollinglist_entities->getPosition().x , scrollinglist_entities->getPosition().y + scrollinglist_entities->getSize().y + 10));



		// Set "Set Entity Button" Characteristics
		
		set_erase_entity_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - set_erase_entity_button->getSize().x/2, entity_id_inputbar->getPosition().y + entity_id_inputbar->getSize().y + 10));


		if(set_erase_entity_button->isClicked())
		{
			set_erase_entity_button->setText("Set");
			*set_or_erase_option = true;
		}
		else
		{
			set_erase_entity_button->setText("Erase");
			*set_or_erase_option = false;
		}
	}
}




void SaveTab(Tab* tab, ArgTab* argtab)
{

	Map** map_to_save = (((ArgSaveTab*)(argtab))->map_to_save); 
	const std::vector<Entity*>* ptr_entities_to_save = (((ArgSaveTab*)(argtab))->ptr_entities_to_save);
	const std::string* ptr_texture_name_file = (((ArgSaveTab*)(argtab))->ptr_texture_name_file);
	Tile** ptr_tileset_to_save = (((ArgSaveTab*)(argtab))->ptr_tileset_to_save);
	const int* nbr_tiles = (((ArgSaveTab*)(argtab))->nbr_tiles);
	int* save_state = (((ArgSaveTab*)(argtab))->save_state);
	std::string** ptr_text_event = ((ArgSaveTab*)(argtab))->text_event_location;
	

	InputBar* file_name_entry = NULL;
	Button* save_button = NULL;
	Button* cancel_button = NULL;


	if(!tab->initialized)
	{
		// Initialization SaveInputbar
		file_name_entry = new InputBar("file_name_entry", 30, tab->getSize().x - 20, InputBar::ALPHANUMERICAL, ptr_text_event, "Map name");
		file_name_entry->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 5));


		// Initialization Save Button
		save_button = new Button("save_button", Button::ONE_STATE, "Save Map", BOXWINDOW_SIZE_CHARACTER);

		// Initialization Cancel Button
		cancel_button = new Button("cancel_button", Button::ONE_STATE, "Cancel", BOXWINDOW_SIZE_CHARACTER);

		cancel_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - (cancel_button->getSize().x + save_button->getSize().x + 4)/2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));
		save_button->setSize(cancel_button->getSize());
		save_button->setPosition(sf::Vector2f(cancel_button->getPosition().x + cancel_button->getSize().x + 2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));

	
		tab->addObject(file_name_entry);
		tab->addObject(save_button);
		tab->addObject(cancel_button);

		tab->initialized = true;
	
	}
	else
	{
		file_name_entry = dynamic_cast<InputBar*>(tab->getObject("file_name_entry"));
		save_button = dynamic_cast<Button*>(tab->getObject("save_button"));
		cancel_button = dynamic_cast<Button*>(tab->getObject("cancel_button"));

		file_name_entry->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 5));
		file_name_entry->setSize(sf::Vector2f(tab->getSize().x - 20, file_name_entry->getSize().y));

		cancel_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - (cancel_button->getSize().x + save_button->getSize().x + 4)/2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));
		save_button->setPosition(sf::Vector2f(cancel_button->getPosition().x + cancel_button->getSize().x + 2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));

		if(save_button->isClicked())
		{
			std::string file_name=file_name_entry->getValue();
			std::size_t tmp_space;

			while( (tmp_space=file_name.find(" "))!= std::string::npos) // To avoid space in file name 
				file_name.erase(tmp_space,1);

			file_name=file_name+".map";

			saveMap(*map_to_save, *ptr_entities_to_save, file_name_entry->getValue(), file_name, *ptr_texture_name_file, *ptr_tileset_to_save, *nbr_tiles);
		}
		
		if(save_button->isClicked() || cancel_button->isClicked())
		{
			*save_state = 2;
		}

	}
}







void LoadTab(Tab* tab, ArgTab* argtab)
{
	Map** map_to_load = (((ArgLoadTab*)(argtab))->map_to_load); 
	std::vector<Entity*>* ptr_entities_to_load = (((ArgLoadTab*)(argtab))->ptr_entities_to_load);
	std::string* ptr_texture_name_file = (((ArgLoadTab*)(argtab))->ptr_texture_name_file);
	sf::Texture** texture_to_load = (((ArgLoadTab*)(argtab))->texture_to_load);
	Tile** ptr_tileset_to_load = (((ArgLoadTab*)(argtab))->ptr_tileset_to_load);
	int* nbr_tiles = (((ArgLoadTab*)(argtab))->nbr_tiles);
	ResourcesManager** ptr_resources_manager = (((ArgLoadTab*)(argtab))->ptr_resources_manager);
	int* load_state = (((ArgLoadTab*)(argtab))->load_state);
	std::string** ptr_text_event = ((ArgLoadTab*)(argtab))->text_event_location;
	

	InputBar* file_name_entry = NULL;
	Button* load_button = NULL;
	Button* cancel_button = NULL;
	TextZone* error_text_zone = NULL;


	if(!tab->initialized)
	{
		// Initialization SaveInputbar
		file_name_entry = new InputBar("file_name_entry", 30, tab->getSize().x - 20, InputBar::ALPHANUMERICAL, ptr_text_event, "Map name");
		file_name_entry->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 5));

		// Initialization Save Button
		load_button = new Button("load_button", Button::ONE_STATE, "Load Map", BOXWINDOW_SIZE_CHARACTER);

		// Initialization Cancel Button
		cancel_button = new Button("cancel_button", Button::ONE_STATE, "Cancel", BOXWINDOW_SIZE_CHARACTER);

		// Initialization Error Text Zone
		error_text_zone = new TextZone("error_text_zone", BOXWINDOW_SIZE_CHARACTER, "", false);


		cancel_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - (cancel_button->getSize().x + load_button->getSize().x + 4)/2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));
		load_button->setSize(sf::Vector2f(load_button->getSize().x,cancel_button->getSize().y));
		load_button->setPosition(sf::Vector2f(cancel_button->getPosition().x + cancel_button->getSize().x + 2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));
		
		error_text_zone->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - error_text_zone->getSize().x/2, cancel_button->getPosition().y + cancel_button->getSize().y + 5));
	
		tab->addObject(file_name_entry);
		tab->addObject(load_button);
		tab->addObject(cancel_button);
		tab->addObject(error_text_zone);

		tab->initialized = true;
	
	}
	else
	{
		file_name_entry = dynamic_cast<InputBar*>(tab->getObject("file_name_entry"));
		load_button = dynamic_cast<Button*>(tab->getObject("load_button"));
		cancel_button = dynamic_cast<Button*>(tab->getObject("cancel_button"));
		error_text_zone = dynamic_cast<TextZone*>(tab->getObject("error_text_zone"));
	
		int result_load = 0;
		if(load_button->isClicked())
		{
			result_load = loadMap(map_to_load, ptr_entities_to_load, file_name_entry->getValue(), ptr_texture_name_file, texture_to_load, ptr_tileset_to_load, nbr_tiles, *ptr_resources_manager);
		}
		
		if((load_button->isClicked() && result_load >= 0) || cancel_button->isClicked())
		{
			*load_state = 2;
		}
		else if(load_button->isClicked())
		{
			error_text_zone->setText("Could not load the map !");
			error_text_zone->setColor(sf::Color(255,25,25,255));
			error_text_zone->update();
			error_text_zone->setSizeToText();
		}

		file_name_entry->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 5));
		file_name_entry->setSize(sf::Vector2f(tab->getSize().x - 20, file_name_entry->getSize().y));

		cancel_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - (cancel_button->getSize().x + load_button->getSize().x + 4)/2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));
		
		load_button->setPosition(sf::Vector2f(cancel_button->getPosition().x + cancel_button->getSize().x + 2, file_name_entry->getPosition().y + file_name_entry->getSize().y + 5));

		error_text_zone->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - error_text_zone->getSize().x/2, cancel_button->getPosition().y + cancel_button->getSize().y + 5));
	}
}












void NewMapTab(Tab* tab, ArgTab* argtab)
{
	Map** new_map = (((ArgNewMapTab*)(argtab))->new_map); 
	std::vector<Entity*>* ptr_new_entities = (((ArgNewMapTab*)(argtab))->ptr_new_entities);
	std::string* ptr_texture_name_file = (((ArgNewMapTab*)(argtab))->ptr_texture_name_file);
	sf::Texture** new_texture = (((ArgNewMapTab*)(argtab))->new_texture);
	ResourcesManager* ptr_resources_manager = (((ArgNewMapTab*)(argtab))->ptr_resources_manager);
	int* new_map_state = (((ArgNewMapTab*)(argtab))->new_map_state);
	std::string** ptr_text_event = ((ArgNewMapTab*)(argtab))->text_event_location;
	

	InputBar* tileset_texture_file_entry = NULL;
	InputBar* height_layers_entry = NULL;
	InputBar* size_x_entry = NULL;
	InputBar* size_y_entry = NULL;
	InputBar* size_tile_entry = NULL;
	Button* create_button = NULL;
	Button* cancel_button = NULL;
	TextZone* error_text_zone = NULL;


	if(!tab->initialized)
	{
		// Initialization Tileset texture entry
		tileset_texture_file_entry = new InputBar("tileset_texture_file_entry", BOXWINDOW_SIZE_CHARACTER, 280, InputBar::ALPHANUMERICAL_SPEC_CHARACTERS, ptr_text_event, "Tileset texture file");
	
		// Initialization Height layers entry
		height_layers_entry = new InputBar("height_layers_entry", BOXWINDOW_SIZE_CHARACTER, 90, InputBar::NUMERICAL, ptr_text_event, "Height");
		//height_layers_entry->setState(InterfaceObject::NONE);


		// Initialization Size x entry
		size_x_entry = new InputBar("size_x_entry", BOXWINDOW_SIZE_CHARACTER, 90, InputBar::NUMERICAL, ptr_text_event, "Size X");

		// Initialization Size y entry
		size_y_entry = new InputBar("size_y_entry", BOXWINDOW_SIZE_CHARACTER, 90, InputBar::NUMERICAL, ptr_text_event, "Size Y");

		// Initialization Size tile entry
		size_tile_entry = new InputBar("size_tile_entry", BOXWINDOW_SIZE_CHARACTER, 90, InputBar::NUMERICAL, ptr_text_event, "Size Tile");

		// Initialization Save Button
		create_button = new Button("create_button", Button::ONE_STATE, "Create", BOXWINDOW_SIZE_CHARACTER);

		// Initialization Cancel Button
		cancel_button = new Button("cancel_button", Button::ONE_STATE, "Cancel", BOXWINDOW_SIZE_CHARACTER);

		// Initialization Error Text Zone
		error_text_zone = new TextZone("error_text_zone", BOXWINDOW_SIZE_CHARACTER, "", false);



		tab->addObject(tileset_texture_file_entry);
		tab->addObject(height_layers_entry);
		tab->addObject(size_x_entry);
		tab->addObject(size_y_entry);
		tab->addObject(size_tile_entry);
		tab->addObject(create_button);
		tab->addObject(cancel_button);
		tab->addObject(error_text_zone);

		tab->initialized = true;

	}
	else
	{
		tileset_texture_file_entry = dynamic_cast<InputBar*>(tab->getObject("tileset_texture_file_entry"));
		height_layers_entry = dynamic_cast<InputBar*>(tab->getObject("height_layers_entry"));
		size_x_entry = dynamic_cast<InputBar*>(tab->getObject("size_x_entry"));
		size_y_entry = dynamic_cast<InputBar*>(tab->getObject("size_y_entry"));
		size_tile_entry = dynamic_cast<InputBar*>(tab->getObject("size_tile_entry"));
		create_button = dynamic_cast<Button*>(tab->getObject("create_button"));
		cancel_button = dynamic_cast<Button*>(tab->getObject("cancel_button"));
		error_text_zone = dynamic_cast<TextZone*>(tab->getObject("error_text_zone"));


	}

	tileset_texture_file_entry->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - tileset_texture_file_entry->getSize().x/2, tab->getPosition().y + tab->getTitleSize() + 10));

	height_layers_entry->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - (height_layers_entry->getSize().x + size_x_entry->getSize().x + size_y_entry->getSize().x + 10)/2 , tileset_texture_file_entry->getPosition().y + tileset_texture_file_entry->getSize().y + 10));
	
	size_x_entry->setPosition(sf::Vector2f(height_layers_entry->getPosition().x + height_layers_entry->getSize().x + 5, height_layers_entry->getPosition().y));
	
	size_y_entry->setPosition(sf::Vector2f(size_x_entry->getPosition().x + size_x_entry->getSize().x + 5, size_x_entry->getPosition().y));
	
	size_tile_entry->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - size_tile_entry->getSize().x/2, size_x_entry->getPosition().y + size_x_entry->getSize().y + 10));

	cancel_button->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - (cancel_button->getSize().x + create_button->getSize().x + 4)/2, size_tile_entry->getPosition().y + size_tile_entry->getSize().y + 10));
	create_button->setSize(sf::Vector2f(create_button->getSize().x,cancel_button->getSize().y));
	create_button->setPosition(sf::Vector2f(cancel_button->getPosition().x + cancel_button->getSize().x + 2, cancel_button->getPosition().y));
	

	int height = 0;
	int size_x = 0;
	int size_y = 0; 
	int size_tile = 0;

	bool map_created = false;


	if( strcmp(height_layers_entry->getValue().c_str(), "")!=0 )
	{
		height = atoi(height_layers_entry->getValue().c_str());
		if(height >= 100) { height_layers_entry->setValue("99"); height = atoi(height_layers_entry->getValue().c_str()); }

	} 

	if( strcmp(size_x_entry->getValue().c_str(), "")!=0 )
	{
		size_x = atoi(size_x_entry->getValue().c_str());
		if(size_x >= 10000) { size_x_entry->setValue("9999"); size_x = atoi(size_x_entry->getValue().c_str()); }
	} 

	if( strcmp(size_y_entry->getValue().c_str(), "")!=0 )
	{
		size_y = atoi(size_y_entry->getValue().c_str());
		if(size_y >= 10000) { size_y_entry->setValue("9999"); size_y = atoi(size_y_entry->getValue().c_str()); }
	}

	if( strcmp(size_tile_entry->getValue().c_str(), "")!=0 )
	{
		size_tile = atoi(size_tile_entry->getValue().c_str());
		if(size_tile > 100) { size_tile_entry->setValue("100"); size_tile = atoi(size_tile_entry->getValue().c_str()); }
	}

	if(create_button->isClicked())
	{
		if(height * size_x * size_y * size_tile == 0)
		{
			error_text_zone->setText("No component equal to 0 !");
			error_text_zone->setColor(sf::Color(255,25,25,255));
			error_text_zone->update();
			error_text_zone->setSizeToText();
		}
		else if(height*size_x*size_y >= 1000000)
		{
			error_text_zone->setText("The map is too big (> 1.000.000 Tiles) !");
			error_text_zone->setColor(sf::Color(255,25,25,255));
			error_text_zone->update();
			error_text_zone->setSizeToText();
		}
		else if(((*new_texture)=ptr_resources_manager->getOrAddTexture(tileset_texture_file_entry->getValue())) == NULL)
		{
			error_text_zone->setText("Texture was not loaded !");
			error_text_zone->setColor(sf::Color(255,25,25,255));
			error_text_zone->update();
			error_text_zone->setSizeToText();

			map_created = true;
		}
		else
		{
			// Set string
			*ptr_texture_name_file = tileset_texture_file_entry->getValue();

			// Clear entities
			for(std::vector<Entity*>::iterator it = ptr_new_entities->begin(); it != ptr_new_entities->end(); it++)
				delete (*it);
			ptr_new_entities->clear();

			// Creation of the map
			*new_map = new Map(sf::Vector2f(size_x, size_y), height, size_tile);


			error_text_zone->setText("Map Created !");
			error_text_zone->setColor(sf::Color(25,255,25,255));
			error_text_zone->update();
			error_text_zone->setSizeToText();

			map_created = true;
		}
	}

	error_text_zone->setPosition(sf::Vector2f(tab->getPosition().x + tab->getSize().x/2 - error_text_zone->getSize().x/2, cancel_button->getPosition().y + cancel_button->getSize().y + 25));


	if((create_button->isClicked() && map_created) || cancel_button->isClicked())
	{
		*new_map_state = 2;
	}
}
