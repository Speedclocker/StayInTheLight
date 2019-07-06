#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Map.h"
#include "saveMap.h"
#include "BoxWindow.h"
#include "tools.h"
#include "functionTabs.h"	

#define FULLSCREEN false


std::string* PTR_EVENT_TEXT_ENTERED;


void windowInside(sf::RenderWindow* window, sf::Texture* texture_tileset, Tile** tiles, int* nbr_avail_tiles, int* size_tile, Tile* target_tile)
{
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	//Gère les fenêtres internes à l'application
	static BoxWindow editWindow(sf::Vector2f(200, 300), window);
	static BoxWindow infoWindow(sf::Vector2f(200, 300), window);

	static bool initialized = false;
	static const BoxWindow* mouse_hover = NULL;
	static const BoxWindow* mouse_click = NULL;

	static std::vector<BoxWindow*> box_windows; 

	if(!initialized)
	{
		// -- Initialization editWindow --
		editWindow.setPositionWindow(sf::Vector2f(window->getView().getSize().x - editWindow.getSize().x - 100, 500));
		editWindow.setMinSize(sf::Vector2f(200, 400));
		editWindow.setMaxSize(sf::Vector2f(texture_tileset->getSize().x + 10, 800));

		// Initialization tab Tileset
		ArgTilesetWindow arg = ArgTilesetWindow{texture_tileset, tiles, nbr_avail_tiles, size_tile, target_tile};
		Tab* tab = new Tab("Tileset", window, TilesetWindow, (ArgTab*)&arg, sizeof(ArgTilesetWindow));
		editWindow.addTab(tab);
		editWindow.focusTab(tab);

		// Initialization tab Entities
		Tab* tab2= new Tab("Entities", window, NULL, NULL, sizeof(NULL));
		editWindow.addTab(tab2);
		
		editWindow.update();

		box_windows.push_back(&editWindow);




		// -- Initialization infoWindow --
		/*
		infoWindow.setPositionWindow(sf::Vector2f(window->getView().getSize().x - editWindow.getSize().x - 300, 100));
		infoWindow.setMinSize(sf::Vector2f(200, 200));
		infoWindow.setMaxSize(sf::Vector2f(200, 200));

		Tab* tab3= new Tab("Infos", window, NULL, NULL, sizeof(NULL));
		infoWindow.addTab(tab3);
		infoWindow.focusTab(tab3);

		infoWindow.update();

		box_windows.push_back(&infoWindow);
		*/

		initialized = true;
	}



	bool no_BW_act = true; 
	bool focus_change = false;
	for(std::vector<BoxWindow*>::iterator it = box_windows.begin(); it != box_windows.end(); it++)
	{
		sf::Rect<float> window_rect = sf::Rect<float>((*it)->getPosition() - sf::Vector2f(5,5), (*it)->getSize() + sf::Vector2f(10,10));
		if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && window_rect.contains(mousePos))
		{
			mouse_hover = (*it);
			no_BW_act = false;
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_hover==(*it))
		{
			mouse_click = (*it);
			(*it)->setFocus();
			focus_change = true;
			mouse_hover = NULL;
			no_BW_act = false;
		}
	}
	if(no_BW_act)
	{
		// If nothing was done on the box windows
		if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			mouse_hover = NULL;
			mouse_click = NULL;	
		}
		else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_click == NULL)
			unFocus_boxwindow();
	}

	if(focus_change)
	{
		// If the window is focused, put it at the end of the vector
		std::vector<BoxWindow*>::iterator it = find(box_windows.begin(), box_windows.end(), BOXWINDOW_FOCUS_WINDOW);
		box_windows.erase(it);
		box_windows.push_back(BOXWINDOW_FOCUS_WINDOW);
	}


	for(std::vector<BoxWindow*>::iterator it = box_windows.begin(); it != box_windows.end(); it++)
	{
		// Interact and draw in the focus order
		(*it)->interactsWithUser();
		(*it)->Function();	
		(*it)->update();

		window->draw(**it);
	}
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

	//Pointeur qui stocke les entrées texte
	PTR_EVENT_TEXT_ENTERED=NULL;

	//Initialisation des tiles
	int nbr_tiles=(texture_file.getSize().x/size_tile)*(texture_file.getSize().y/size_tile);
	Tile* available_tiles= (Tile*)malloc(nbr_tiles * sizeof(Tile));
	for(int i=0; i<nbr_tiles;i++)
	{
		available_tiles[i].m_pos_text = sf::Vector2f( (i%(int)(texture_file.getSize().x/size_tile))*size_tile,  (i/(int)(texture_file.getSize().x/size_tile))*size_tile );
		available_tiles[i].m_size_text = sf::Vector2f( size_tile, size_tile );
		available_tiles[i].m_collisionable = false;
	}
	Tile buff_tile=available_tiles[0];


	//Création de la vue
	sf::View main_view(sf::Vector2f(size_x*size_tile/2, size_y*size_tile/2), sf::Vector2f(window.getSize()));
	main_view.setCenter(0,0);
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

	//Main Loop
	while(window.isOpen())
	{
		window.clear();

        // Attente des évenements
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) )
				window.close();

			if( (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space) )
			{
				custom_map->setSize(custom_map->getSize().x + 1, custom_map->getSize().y - 1);
				main_view.setCenter(sf::Vector2f(custom_map->getTileSize() * custom_map->getSize().x/2, custom_map->getTileSize() * custom_map->getSize().y/2));

			}
				//if((int)window.getSize().x%2==1) window.setSize(sf::Vector2u(window.getSize().x-1, window.getSize().y));
				//if((int)window.getSize().y%2==1) window.setSize(sf::Vector2u(window.getSize().x, window.getSize().y-1));
			if (event.type == sf::Event::Resized)
			{
				
				main_view.setSize(event.size.width, event.size.height);
				
				
				main_view.setCenter(sf::Vector2f(custom_map->getTileSize() * custom_map->getSize().x/2, custom_map->getTileSize() * custom_map->getSize().y/2));
				window.setView(main_view);
			}

			if(event.type == sf::Event::TextEntered)
			{
				// If some text is entered
				if (event.text.unicode < 128 && PTR_EVENT_TEXT_ENTERED!=NULL)
				{
					if(event.text.unicode == 8 && PTR_EVENT_TEXT_ENTERED->length() > 0)
						PTR_EVENT_TEXT_ENTERED->erase(PTR_EVENT_TEXT_ENTERED->end()-1);
					else if(event.text.unicode != 8)
	        			*PTR_EVENT_TEXT_ENTERED += event.text.unicode;
				}
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
		transparency = height_settings_command(&window, custom_map, &chosen_height);

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
		save_map_command(custom_map, texture_file_name, available_tiles, nbr_tiles);



		//Pour charger une map
		if(load_map_command(&window, &custom_map, &texture_file, &available_tiles, &nbr_tiles, FULLSCREEN)==1)
		{
			size_tile=custom_map->getTileSize();
			buff_tile=available_tiles[0];
		}


		// Affichage des éléments

		// BoxWindows
		windowInside(&window, &texture_file, &available_tiles, &nbr_tiles, &size_tile, &buff_tile);

		window.display();

	}

	delete custom_map;

	free(available_tiles);

	return 0;

}
