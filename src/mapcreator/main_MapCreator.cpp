#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Map.h"
#include "tools.h"
#include "GUI.h"
#include "ResourcesManagement.h"

#define FULLSCREEN true

	#define DEFAULT_SIZE_H 3
	#define DEFAULT_SIZE_X 30
	#define DEFAULT_SIZE_Y 30
	#define DEFAULT_SIZE_TILE 16
	#define DEFAULT_TEXTURE_FILE_NAME "data/imgs/tileset.png"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	// Creation of the main window
	sf::RenderWindow window( ( (FULLSCREEN) ? sf::VideoMode(DEFAULT_SIZE_TILE*DEFAULT_SIZE_X, DEFAULT_SIZE_TILE*DEFAULT_SIZE_Y) : sf::VideoMode((DEFAULT_SIZE_TILE*DEFAULT_SIZE_X<1200) ? DEFAULT_SIZE_TILE*DEFAULT_SIZE_X : 1200 , (DEFAULT_SIZE_TILE*DEFAULT_SIZE_Y<720) ? DEFAULT_SIZE_TILE*DEFAULT_SIZE_Y : 720)),
							  "MapCreator", ( (FULLSCREEN) ? sf::Style::Fullscreen : sf::Style::Default ) );
	window.setFramerateLimit(60);


	// Creation of the resources manager
	ResourcesManager resources_manager;


	// Creation of the view
	sf::View main_view(sf::Vector2f(DEFAULT_SIZE_X*DEFAULT_SIZE_TILE/2, DEFAULT_SIZE_Y*DEFAULT_SIZE_TILE/2), sf::Vector2f(window.getSize()));
	window.setView(main_view);


	// Creation of the map
	Map* custom_map = new Map(sf::Vector2f(DEFAULT_SIZE_X, DEFAULT_SIZE_Y), DEFAULT_SIZE_H, DEFAULT_SIZE_TILE);


	// Vector of entities
	std::vector<Entity*> entities;


	// Creation of the user interface and all interactions
	MappingGUI* mappingGUI;
	try { mappingGUI = new MappingGUI(&window, &custom_map, &entities, &resources_manager, DEFAULT_TEXTURE_FILE_NAME); }
	catch (std::string s) 
	{ 
		std::cerr << "MappingGUI error" << std::endl; mappingGUI = NULL; 
		delete custom_map;
		return -1;
	}


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
				if (event.text.unicode < 128 && event.text.unicode != 10 && event.text.unicode != 13 && mappingGUI->getStringPtrToTextEvent()!=NULL)
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
		delete (*it);

	return 0;
}