#include "functionTabs.h"
#include "saveMap.h"


#define FONT_FILE "../data/fonts/AldoTheApache.ttf"
#define ENTITIES_FOLDER "entities/"
#define BOXWINDOW_SIZE_CHARACTER 16


void TilesetTab(Tab* tab, ArgTab* argtab)
{
	sf::Texture* texture = ((ArgTilesetTab*)(argtab))->texture;
	Tile** tiles = ((ArgTilesetTab*)(argtab))->tiles;
	const int* nbr_avail_tiles= ((ArgTilesetTab*)(argtab))->nbr_avail_tiles;
	const int* size_tile = ((ArgTilesetTab*)(argtab))->size_tile;
	Tile* target_tile = ((ArgTilesetTab*)(argtab))->target_tile;
	std::string** ptr_text_event = ((ArgTilesetTab*)(argtab))->text_event_location;

	sf::Rect<float> zone_tileset(sf::Vector2f(0, 0), sf::Vector2f(tab->getSize().x - 10, texture->getSize().y )); // Fait dépendre la taille du tileset de la fenêtre

	if(!tab->initialized)
	{
		// Outils interface -- Barre de slide pour le tileset et miniature tileset
		SlideBar *ptr_slide;
		TilesetSelect *ptr_tileset;
		Button *ptr_button;
		InputBar *ptr_inputbar1;
		InputBar *ptr_inputbar2;


		//Initialisation du tileset
		ptr_tileset = new TilesetSelect("tileset", texture, zone_tileset, tiles, nbr_avail_tiles, size_tile, target_tile);
		ptr_tileset->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));
		ptr_tileset->setZone(zone_tileset);


		//Initialisation Slidebar1 
		ptr_slide = new SlideBar("slidebar1", 0, ptr_tileset->getMaxZonePos().x);
		ptr_slide->setSize(sf::Vector2f(ptr_tileset->getZone().width, 15));
		ptr_slide->setType(SlideBar::HORIZONTAL);
		ptr_slide->setPosition(sf::Vector2f(ptr_tileset->getPosition().x , ptr_tileset->getPosition().y + ptr_tileset->getZone().height + 5) );


		//Initialisation InputBar
		ptr_inputbar1 = new InputBar("inputbar1", 16, 100, InputBar::NUMERICAL, ptr_text_event);
		ptr_inputbar1->setPosition(ptr_slide->getPosition() + sf::Vector2f(0, ptr_slide->getSize().y + 10));
		
		ptr_inputbar2 = new InputBar("inputbar2", 16, 100, InputBar::NUMERICAL, ptr_text_event);
		ptr_inputbar2->setPosition(ptr_inputbar1->getPosition() + sf::Vector2f(0, ptr_inputbar1->getSize().y + 10));

		//Initialisation Button
		ptr_button = new Button("button1", "Change", 16);
		ptr_button->setPosition(ptr_inputbar1->getPosition() + sf::Vector2f(ptr_inputbar1->getSize().x + 30, 0));


		//Ajout des objets à l'onglet
		tab->addObject(ptr_slide);
		tab->addObject(ptr_tileset);
		tab->addObject(ptr_inputbar1);
		tab->addObject(ptr_inputbar2);
		tab->addObject(ptr_button);

		tab->initialized = true;
	}

	else	
	{
		//Initialization objects
		TilesetSelect* tileset = dynamic_cast<TilesetSelect*>(tab->getObject("tileset"));
		SlideBar* slidebar1 = dynamic_cast<SlideBar*>(tab->getObject("slidebar1"));
		InputBar* inputbar1 = dynamic_cast<InputBar*>(tab->getObject("inputbar1"));
		InputBar* inputbar2 = dynamic_cast<InputBar*>(tab->getObject("inputbar2"));
		Button* button1 = dynamic_cast<Button*>(tab->getObject("button1"));


		//Paramétrage du tileset
		tileset->setZone(zone_tileset);
		tileset->setZone(sf::Rect<float>(slidebar1->getCurrentValue(), tileset->getZone().top, tileset->getZone().width, tileset->getZone().height));
		tileset->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));


		//Paramétrage des slidebar
		slidebar1->setPosition(sf::Vector2f(tileset->getPosition().x , tileset->getPosition().y + tileset->getZone().height + 5) );
		slidebar1->setSize(sf::Vector2f(tileset->getZone().width, 15));
		slidebar1->setRatioCursorBar((float)tileset->getZone().width/(float)(tileset->getMaxZonePos().x + tileset->getZone().width));
		slidebar1->setMaxValue(tileset->getMaxZonePos().x);


		//Paramétrage du InputBar
		inputbar1->setPosition(slidebar1->getPosition() + sf::Vector2f(0, slidebar1->getSize().y + 10));
		inputbar2->setPosition(inputbar1->getPosition() + sf::Vector2f(0, inputbar1->getSize().y + 10));

		//Paramétrage du button
		button1->setPosition(inputbar1->getPosition() + sf::Vector2f(inputbar1->getSize().x + 30, 0));


		//Stocke la valeur du tile cible
		*target_tile = tileset->getChosenTile();
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


	EntityDisplayer *ptr_spritedisplayer;
	ScrollingList *ptr_scrollinglist_entities;

	static std::string chosen_entity_string;

	static sf::Texture texture;

	if(!tab->initialized)
	{
		//Initialization EntityDisplayer
		
		ptr_spritedisplayer = new EntityDisplayer("spritedisplayer", true);
		ptr_spritedisplayer->setSize(sf::Vector2f(tab->getSize().x - 20, tab->getSize().x - 20));
		ptr_spritedisplayer->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 10));


		//Initialization ScrollingList
		ptr_scrollinglist_entities = new ScrollingList("scrollinglist_entities", BOXWINDOW_SIZE_CHARACTER, 100, tab->getSize().x - 20);
		ptr_scrollinglist_entities->setPosition(sf::Vector2f(ptr_spritedisplayer->getPosition().x , ptr_spritedisplayer->getPosition().y + ptr_spritedisplayer->getSize().y + 10) );

		// Adding all values to the ScrollingList
		for(std::vector<std::string>::iterator it=list_entities.begin(); it!=list_entities.end(); it++)
			ptr_scrollinglist_entities->addValue(*it);
		

		// Add object to the tab
		tab->addObject(ptr_spritedisplayer);
		tab->addObject(ptr_scrollinglist_entities);


		chosen_entity_string = ptr_scrollinglist_entities->getCurrentValue();

		ptr_spritedisplayer->setEntity(*current_entity);

		tab->initialized = true;
	}

	else
	{
		EntityDisplayer* spritedisplayer = dynamic_cast<EntityDisplayer*>(tab->getObject("spritedisplayer"));
		ScrollingList* scrollinglist_entities = dynamic_cast<ScrollingList*>(tab->getObject("scrollinglist_entities"));

		// Set sprite displayer position
		spritedisplayer->setPosition(sf::Vector2f(tab->getPosition().x + 10, tab->getPosition().y + tab->getTitleSize() + 10));

		// Set scrolling list position
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

	}
}