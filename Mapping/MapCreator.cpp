#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Map.h"
#include "saveMap.h"
#include "BoxWindow.h"
#include "tools.h"
#include "functionTabs.h"	

#define FULLSCREEN true


void windowInside(sf::RenderWindow* window, sf::Texture* texture_tileset, Tile** tiles, int* nbr_avail_tiles, int* size_tile, Tile* target_tile)
{
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	//Gère les fenêtres internes à l'application
	static BoxWindow tmp_boxwindow(sf::Vector2f(200, 300), window);
	
	static bool initialized = false;
	static const BoxWindow* mouse_hover = NULL;
	static const BoxWindow* mouse_click = NULL;

	
	if(!initialized)
	{
		ArgTilesetWindow arg = ArgTilesetWindow{texture_tileset, tiles, nbr_avail_tiles, size_tile, target_tile};
		
		Tab* tab = new Tab("Tileset", window, TilesetWindow, (ArgTab*)&arg, sizeof(ArgTilesetWindow));
		
		tmp_boxwindow.addTab(tab);
		tmp_boxwindow.setPositionWindow(sf::Vector2f(window->getView().getSize().x - tmp_boxwindow.getSize().x - 100, 100));

		tmp_boxwindow.setMinSize(sf::Vector2f(200, 400));

		tmp_boxwindow.setMaxSize(sf::Vector2f(texture_tileset->getSize().x + 10, 800));
		tmp_boxwindow.update();

		initialized = true;
	}
	
	sf::Rect<float> tmp_window_rect = sf::Rect<float>(tmp_boxwindow.getPosition()-sf::Vector2f(5,5), tmp_boxwindow.getSize()+sf::Vector2f(10,10));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && tmp_window_rect.contains(mousePos))
	{
		mouse_hover = &tmp_boxwindow;
	}
	else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !tmp_window_rect.contains(mousePos) && mouse_click == NULL)
		unFocus_boxwindow();
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mouse_hover = NULL;
		mouse_click = NULL;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_hover==&tmp_boxwindow)
	{
		mouse_click = &tmp_boxwindow;
		tmp_boxwindow.setFocus();
		mouse_hover = NULL;
	}
	


	//if(tmp_boxwindow.hasFocus())
	tmp_boxwindow.interactsWithUser();


	tmp_boxwindow.Function();	

	tmp_boxwindow.update();

	window->draw(tmp_boxwindow);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	if(argc!=6)
	{
		std::cout << "Lancez MapCreator de la manière suivante ./MapCreator [SIZE_MAP_H] [SIZE_MAP_X] [SIZE_MAP_Y] [SIZE_TILE] [NAME_TEXT_FILE]" << std::endl;
		return -1;
	}

	int size_h = atoi(argv[1]);
	int size_x = atoi(argv[2]);
	int size_y = atoi(argv[3]);
	int size_tile = atoi(argv[4]);
	std::string texture_file_name = argv[5];

	sf::Texture texture_file;
	
	if( texture_file.loadFromFile(texture_file_name) == false)
	{
		std::cout << "Le fichier texture n'a pas pu être chargé." << std::endl;
		return -1;
	}
	

	if(size_x*size_y*size_h > 1000000)
	{
		std::cout << "La map est trop grande (<1000000 tiles)" << std::endl;
		return -1;
	}


	//Texte à afficher au lancement
	introText();



	//Création de la fenêtre principale
	sf::RenderWindow window( ( (FULLSCREEN) ? sf::VideoMode(size_tile*size_x, size_tile*size_y) : sf::VideoMode((size_tile*size_x<1200) ? size_tile*size_x : 1200 , (size_tile*size_y<720) ? size_tile*size_y : 720)),
							  "MapCreator", ( (FULLSCREEN) ? sf::Style::Fullscreen : sf::Style::Default ) );
	window.setFramerateLimit(120);


	//Initialisation des tiles
	int nbr_tiles=(texture_file.getSize().x/size_tile)*(texture_file.getSize().y/size_tile);
	Tile* available_tiles= (Tile*)malloc(nbr_tiles * sizeof(Tile));
	for(int i=0; i<nbr_tiles;i++)
	{
		available_tiles[i].m_pos_text = sf::Vector2f( (i%(texture_file.getSize().x/size_tile))*size_tile,  (i/(texture_file.getSize().x/size_tile))*size_tile );
		available_tiles[i].m_size_text = sf::Vector2f( size_tile, size_tile );
		available_tiles[i].m_collisionable = false;
	}
	Tile buff_tile=available_tiles[0];


	//Création de la vue
	sf::View main_view(sf::Vector2f(size_x*size_tile/2, size_y*size_tile/2), sf::Vector2f(window.getSize()));
	window.setView(main_view);



	//Création de la map
	Map* custom_map=new Map(sf::Vector2f(size_x, size_y), size_h, size_tile);
	custom_map->setTexture(&texture_file);

	//Zone Map
	sf::RectangleShape map_zone(sf::Vector2f(custom_map->getSize().x*custom_map->getTileSize(), custom_map->getSize().y*custom_map->getTileSize()));
	map_zone.setFillColor(sf::Color(0,0,0,0));
	map_zone.setOutlineThickness(2);
	map_zone.setOutlineColor(sf::Color::Red);
	int chosen_height=0;


	
	bool transparency=false;

	//Boucle principale
	while(window.isOpen())
	{
		// MAIN WINDOW

		window.clear();

        // Attente des évenements
		sf::Event event;// tileset_event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) )
			{
				window.close();
				//tileset_window.close();
			}
			
			if (event.type == sf::Event::Resized)
			{
				main_view.setSize(event.size.width, event.size.height);
				main_view.setCenter(sf::Vector2f(custom_map->getTileSize() * custom_map->getSize().x/2, custom_map->getTileSize() * custom_map->getSize().y/2));
				window.setView(main_view);
			}
		}
	

		//Dessine la limite de la map
		map_zone.setSize(sf::Vector2f(custom_map->getSize().x*custom_map->getTileSize(), custom_map->getSize().y*custom_map->getTileSize()));

		
		// Défini si l'affichage est en mode transparent ou normal		
		if(transparency)
			custom_map->update_transparency(chosen_height);
		else
			custom_map->update();


		//Dessine la map
		window.draw(*custom_map);
		window.draw(map_zone);



		// Changement de hauteur
		transparency=height_settings(&window, custom_map, &chosen_height);



		//Si l'utilisateur utilise la fenêtre principale
		if(window.hasFocus())
		{
			//Contrôle de la vue
			control_view(&window, &main_view, custom_map);
			window.setView(main_view);
		}


		//Pour placer les tiles
		if(noFocus_boxwindow())
			set_tile(&window, &texture_file, custom_map, buff_tile, chosen_height);



		//Pour sauvegarder la map
		save_map(custom_map, texture_file_name);



		//Pour charger une map
		if(load_map(&window, &custom_map, &texture_file, &available_tiles, &nbr_tiles, FULLSCREEN)==1)
		{
			size_tile=custom_map->getTileSize();
			buff_tile=available_tiles[0];
		}


		// Affichage des éléments

		// Fenêtre interne de tileset
		windowInside(&window, &texture_file, &available_tiles, &nbr_tiles, &size_tile, &buff_tile);

		window.display();

	}

	delete custom_map;

	free(available_tiles);

	return 0;

}