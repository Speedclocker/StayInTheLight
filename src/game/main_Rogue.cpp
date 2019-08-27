#include <SFML/Graphics.hpp>
#include <ctime>

#include "Interactions.h"
#include "Character.h"
#include "collision.h"
#include "loadContent.h"
#include "ResourcesManagement.h"
#include "Camera.h"





int main()
{

	//Window creation and setting
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height), "Comment", sf::Style::Fullscreen);	
	window.setFramerateLimit(60);	// Framerate limit
	window.setKeyRepeatEnabled(false);	// No repetition of key-event

	bool space=false;	// State variable of attack key



	//Map creation
	Map* map=NULL;


	
	// Entities
	std::vector<Entity*> entities;



	// Resources manager
	ResourcesManager resources_manager;



	// Loading map and Entities
	if(loadMap(&map, &entities, "data/maps/Bigmap.map", &resources_manager)<0)
	{
		std::cerr << "Error while loading of map" << std::endl;
		return -1;
	}
	for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
		map->addEntity(*it);
	


	// Light
	sf::Texture* light = resources_manager.getOrAddTexture("data/imgs/light.png");
	sf::Sprite light_sprite(*light);



	// Hero creation
	Character* hero = NULL;
	bool hero_allocated = false;
	


	// Enemy creation
	Character* enemy = NULL;
	bool enemy_allocated = false;



	// Entities
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if((*it)->getID() == "hero" && (*it)->getType()=="Character") 				hero = (Character*)(*it);
		else if((*it)->getID() == "enemy" && (*it)->getType()=="Character") 		enemy = (Character*)(*it);
	}

	if(hero == NULL)
	{
		hero = new Character("hero", "data/entities/hero_01.ent", &resources_manager, map);
		hero->setPosition(sf::Vector2f(map->getSize().x*map->getTileSize()/2 - hero->getSize().x/2, map->getSize().y*map->getTileSize()/2 - hero->getSize().y/2));
		hero->setHeight(1);
		map->addEntity(hero);
		hero_allocated = true;
	}

	if(enemy == NULL)
	{
		enemy = new Character("enemy", "data/entities/hero_01.ent", &resources_manager, map);
		enemy->setPosition(sf::Vector2f(map->getSize().x*map->getTileSize()/2 - enemy->getSize().x/2,  map->getSize().y*map->getTileSize()/2 - 100 - enemy->getSize().y/2));
		enemy->setHeight(1);
		map->addEntity(enemy);
		enemy_allocated = true;
	}


	// Camera
	Camera camera(&window, true, 2, hero->getPosition(), sf::Vector2f((int)(window.getSize().x*0.4f), (int)(window.getSize().y*0.4f)));
	camera.setFocusPoint(hero->getPosition());


	//Add a potential attack target for hero
	hero->addAvTarget(enemy); 



	// Time DEBUG
	sf::Clock clock_e;
	sf::Time time_e = clock_e.getElapsedTime();

	


	//Main loop
	while(window.isOpen())
	{
		std::cout << "Elapsed time : " << (clock_e.getElapsedTime() - time_e).asMicroseconds() << std::endl;
		time_e = clock_e.getElapsedTime();

		// Refresh Window
		window.clear();



        // Waiting for events
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed || ( (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) )
				window.close();
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
				space=true;

			if (event.type == sf::Event::Resized)
			{
				sf::View main_view;

				// Adapt the view if window has been resized
				main_view.setSize(event.size.width, event.size.height);
				main_view.setCenter(sf::Vector2f(map->getTileSize() * map->getSize().x/2, map->getTileSize() * map->getSize().y/2));
				camera.setSizeView(sf::Vector2f((int)(window.getSize().x*0.4f), (int)(window.getSize().y*0.4f)));
				window.setView(main_view);
			}
		}
		
		

		// Keyboard input are associated to the hero
		character_key_input(hero, &space);



		// Update attack state of the hero
		hero->updateAttack();
		enemy->updateAttack();



		// We apply physics on the objects associated with the current map
		map->physics_entities();



		//Update all entities
		for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
			(*it)->update();



		// Update map
		map->update(); // Update map



		// Drawing the map
		window.draw(*map);


		// Set and draw light
		light_sprite.setPosition(hero->getPosition() + sf::Vector2f(hero->getSize().x/2, hero->getSize().y/2) - sf::Vector2f(light->getSize().x/2, light->getSize().y/2));
		window.draw(light_sprite);



		// Set and update camera
		camera.setFocusPoint(hero->getPosition());
		camera.update();



		// Displaying the window
		window.display();
	}


	// Delete Map
	delete map;



	// Delete Entities
	std::vector<Entity*>::iterator it = entities.begin();
	while(it!=entities.end())
	{
		delete(*it);
		it = entities.erase(it);
	}
	if(hero_allocated) delete hero;
	if(enemy_allocated) delete enemy;


	return 0;
}
