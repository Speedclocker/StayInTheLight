#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Map.h"
#include "tools.h"
#include "GUI.h"
#include "ResourcesManagement.h"

#define FULLSCREEN true
#define ROOT_DIR "../"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	if(argc!=6) { std::cerr << "Lancez MapCreator de la manière suivante ./MapCreator [SIZE_MAP_H] [SIZE_MAP_X] [SIZE_MAP_Y] [SIZE_TILE] [NAME_TEXT_FILE]" << std::endl; return -1; }

	int size_h = atoi(argv[1]);
	int size_x = atoi(argv[2]);
	int size_y = atoi(argv[3]);
	int size_tile = atoi(argv[4]);
	std::string texture_file_name = argv[5];

		
	if(size_x*size_y*size_h > 1000000) { std::cout << "Error : Oversized Map (> 1000000 tiles)" << std::endl; return -1; }

	
	// Creation of the main window
	sf::RenderWindow window( ( (FULLSCREEN) ? sf::VideoMode(size_tile*size_x, size_tile*size_y) : sf::VideoMode((size_tile*size_x<1200) ? size_tile*size_x : 1200 , (size_tile*size_y<720) ? size_tile*size_y : 720)),
							  "MapCreator", ( (FULLSCREEN) ? sf::Style::Fullscreen : sf::Style::Default ) );
	window.setFramerateLimit(60);


	// Creation of the resources manager
	ResourcesManager resources_manager;


	// Creation of the view
	sf::View main_view(sf::Vector2f(size_x*size_tile/2, size_y*size_tile/2), sf::Vector2f(window.getSize()));
	window.setView(main_view);


	// Creation of the map
	Map* custom_map=new Map(sf::Vector2f(size_x, size_y), size_h, size_tile);


	// Vector of entities
	std::vector<Entity*> entities;


	// Creation of the user interface and all interactions
	MappingGUI* mappingGUI = new MappingGUI(&window, custom_map, &entities, &resources_manager, texture_file_name);


	// Information text to display at the beginning
	introText();


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
			else if( (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space) )
			{
				custom_map->setSize(custom_map->getSize().x + 1, custom_map->getSize().y - 1);
				main_view.setCenter(sf::Vector2f(custom_map->getTileSize() * custom_map->getSize().x/2, custom_map->getTileSize() * custom_map->getSize().y/2));

			}
			else if (event.type == sf::Event::Resized)
			{
				main_view.setSize(event.size.width, event.size.height);
					
				main_view.setCenter(sf::Vector2f(custom_map->getTileSize() * custom_map->getSize().x/2, custom_map->getTileSize() * custom_map->getSize().y/2));
				window.setView(main_view);
			}
			else if(event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128 && mappingGUI->getStringPtrToTextEvent()!=NULL)
				{
					if(event.text.unicode == 8 && mappingGUI->getStringPtrToTextEvent()->length() > 0)
						mappingGUI->getStringPtrToTextEvent()->erase(mappingGUI->getStringPtrToTextEvent()->end()-1);
					else if(event.text.unicode != 8)
	        			*mappingGUI->getStringPtrToTextEvent() += event.text.unicode;
				}
			}
		}
	
		// Set interaction with user, update and draw
		mappingGUI->interactWithUser();
		mappingGUI->update();
		mappingGUI->draw();

		window.display();

	}


	delete mappingGUI;
	delete custom_map;

	for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		delete (*it);
		//entities.erase(it);
	}

	return 0;
}