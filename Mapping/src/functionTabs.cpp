#include "functionTabs.h"


void TilesetWindow(Tab* tab, ArgTab* argtab)
{
	sf::Texture* texture = ((ArgTilesetWindow*)(argtab))->texture;
	Tile** tiles = ((ArgTilesetWindow*)(argtab))->tiles;
	int* nbr_avail_tiles= ((ArgTilesetWindow*)(argtab))->nbr_avail_tiles;
	int* size_tile = ((ArgTilesetWindow*)(argtab))->size_tile;
	Tile* target_tile = ((ArgTilesetWindow*)(argtab))->target_tile;

	sf::Rect<float> zone_tileset(sf::Vector2f(0, 0), sf::Vector2f(tab->getSize().x - 10, texture->getSize().y )); // Fait dépendre la taille du tileset de la fenêtre

	if(!tab->initialized)
	{
		// Outils interface -- Barre de slide pour le tileset et miniature tileset
		SlideBar *ptr_slide;
		TilesetSelect *ptr_tileset;
		Button *ptr_button;
		InputBar *ptr_inputbar;


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
		ptr_inputbar = new InputBar("inputbar1", 16, 100, InputBar::NUMERICAL);
		ptr_inputbar->setPosition(ptr_slide->getPosition() + sf::Vector2f(0, ptr_slide->getSize().y + 10));


		//Initialisation Button
		ptr_button = new Button("button1", "Change", 16);
		ptr_button->setPosition(ptr_inputbar->getPosition() + sf::Vector2f(ptr_inputbar->getSize().x + 30, 0));


		//Ajout des objets à l'onglet
		tab->addObject(ptr_slide);
		tab->addObject(ptr_tileset);
		tab->addObject(ptr_inputbar);
		tab->addObject(ptr_button);

		tab->initialized = true;
	}

	if(tab->initialized)
	{
		//Initialization objects
		TilesetSelect* tileset = dynamic_cast<TilesetSelect*>(tab->getObject("tileset"));
		SlideBar* slidebar1 = dynamic_cast<SlideBar*>(tab->getObject("slidebar1"));
		InputBar* inputbar1 = dynamic_cast<InputBar*>(tab->getObject("inputbar1"));
		Button* button1 = dynamic_cast<Button*>(tab->getObject("button1"));


		//Paramétrage du tileset
		tileset->setZone(zone_tileset);
		tileset->setZone(sf::Rect<float>(slidebar1->getCurrentValue(), tileset->getZone().top, tileset->getZone().width, tileset->getZone().height));
		tileset->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));


		//Paramétrage des slidebar
		slidebar1->setPosition(sf::Vector2f(tileset->getPosition().x , tileset->getPosition().y + tileset->getZone().height + 5) );
		slidebar1->setSize(sf::Vector2f(tileset->getZone().width, 15));
		slidebar1->setMaxValue(tileset->getMaxZonePos().x);


		//Paramétrage du InputBar
		inputbar1->setPosition(slidebar1->getPosition() + sf::Vector2f(0, slidebar1->getSize().y + 10));


		//Paramétrage du button
		button1->setPosition(inputbar1->getPosition() + sf::Vector2f(inputbar1->getSize().x + 30, 0));


		//Stocke la valeur du tile cible
		*target_tile = tileset->getChosenTile();
	}

}








/*
void InfoWindow(Tab* tab, ArgTab* argtab)
{
	
	sf::RenderWindow
	sf::Font* font = ((ArgInfoWindow*)(argtab))->font;
	Map* map = ((ArgInfoWindow*)(argtab))->map;

	std::string all_infos="";
	std::string size = "Size : " + + " x " + + " \n";

	all_infos += size;

	sf::Text text_infos(all_infos, *font, 16);
	t
	
}
*/