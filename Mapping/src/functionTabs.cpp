#include "functionTabs.h"


void TilesetWindow(Tab* tab, ArgTab* argtab)
{
	sf::Texture* texture = ((ArgTilesetWindow*)(argtab))->texture;
	Tile* tiles = ((ArgTilesetWindow*)(argtab))->tiles;
	const int* nbr_avail_tiles= ((ArgTilesetWindow*)(argtab))->nbr_avail_tiles;
	const int* size_tile = ((ArgTilesetWindow*)(argtab))->size_tile;
	Tile* target_tile = ((ArgTilesetWindow*)(argtab))->target_tile;

	sf::Rect<float> zone_tileset(sf::Vector2f(0, 0), sf::Vector2f(tab->getSize().x - 10, texture->getSize().y ));

	if(!tab->initialized)
	{
		// Outils interface -- Barre de slide pour le tileset et miniature tileset
		SlideBar *ptr_slide;
		TilesetSelect *ptr_tileset;


		//Initialisation du tileset
		ptr_tileset = new TilesetSelect("tileset", texture, zone_tileset, tiles, *nbr_avail_tiles, *size_tile);
		ptr_tileset->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));
		ptr_tileset->setZone(zone_tileset);


		//Initialisation Slidebar1 
		ptr_slide = new SlideBar("slidebar1", 0, ptr_tileset->getMaxZonePos().x);
		ptr_slide->setSize(sf::Vector2f(ptr_tileset->getZone().width, 15));
		ptr_slide->setType(SlideBar::HORIZONTAL);
		ptr_slide->setPosition(sf::Vector2f(ptr_tileset->getPosition().x , ptr_tileset->getPosition().y + ptr_tileset->getZone().height + 5) );


		//Ajout des objets à l'onglet
		tab->addObject(ptr_slide);
		tab->addObject(ptr_tileset);

		tab->initialized = true;
	}

	if(tab->initialized)
	{
		//Paramétrage du tileset
		TilesetSelect* tileset = dynamic_cast<TilesetSelect*>(tab->getObject("tileset"));
		tileset->setZone(zone_tileset);
		tileset->setPosition(sf::Vector2f(tab->getPosition().x + 5, tab->getPosition().y + tab->getTitleSize() + 5));


		//Paramétrage des slidebar
		SlideBar* slidebar1 = dynamic_cast<SlideBar*>(tab->getObject("slidebar1"));
		slidebar1->setPosition(sf::Vector2f(tileset->getPosition().x , tileset->getPosition().y + tileset->getZone().height + 5) );
		slidebar1->setSize(sf::Vector2f(tileset->getZone().width, 15));
		slidebar1->setMaxValue(tileset->getMaxZonePos().x);


		// Interaction
		slidebar1->interactsWithUser(tab->getRenderWindow());

		tileset->setZone(sf::Rect<float>(slidebar1->getCurrentValue(), tileset->getZone().top, tileset->getZone().width, tileset->getZone().height));
		tileset->interactsWithUser(tab->getRenderWindow());

		*target_tile = tileset->getChosenTile();
	}

}
