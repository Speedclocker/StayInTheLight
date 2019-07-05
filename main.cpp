#include <SFML/Graphics.hpp>
#include <ctime>

#include "Interactions.h"
#include "Character.h"
#include "Collector.h"
#include "collision.h"
#include "loadContent.h"


int main()
{

	//Window creation and setting
	sf::RenderWindow window(sf::VideoMode(640,480), "Comment");	
	window.setFramerateLimit(60);	// Framerate limit
	window.setKeyRepeatEnabled(false);	// No repetition of key-event

	bool space=false;	// State variable of attack key



	//Map creation
	Map* map=NULL;
	sf::Texture texture_map;
	std::string texture_name_file;

	if(loadMap(&map, "data/maps/Test.map", &texture_name_file)<0)
	{
		std::cerr << "Error while loading of map" << std::endl;
		return -1;
	}
	
	if(!texture_map.loadFromFile(texture_name_file)){ std::cerr << "Error while loading of texture \"texture_map\"" << std::endl; return -1; }

	map->setTexture(&texture_map);



	//Texture creation and loading
	sf::Texture texture_link;
	texture_link.loadFromFile("data/imgs/linkmv.png");



	//Character creation
	/*personnage.getSize().y-10*/
	Character personnage(&texture_link, map);
	personnage.setSpeed(2);
	personnage.setHitbox(sf::IntRect(0, 5 , personnage.getSize().x/1.5, 16));
	personnage.setHealth(10);
	personnage.setPosition(sf::Vector2f(window.getSize().x/2-personnage.getSize().x/2, window.getSize().y/2-personnage.getSize().y/2));
	personnage.setHeight(1);
	map->addEntity(&personnage);



	//Mob creation
	Character mob(&texture_link, map);
	mob.setHealth(10);
	mob.setHitbox(sf::IntRect(0, 5 , mob.getSize().x/1.5, 16 /*mob.getSize().y-10*/));
	mob.setPosition(sf::Vector2f(window.getSize().x/2-mob.getSize().x/2, window.getSize().y/2-100-mob.getSize().y/2));
	mob.setHeight(1);
	map->addEntity(&mob);

	//Add a potential attack target for hero
	personnage.addAvTarget(&mob); 


	sf::Texture collectortext;

	Collector tmp_coll(&texture_link, map);
	tmp_coll.setSpeed(2);
	tmp_coll.setHitbox(sf::IntRect(0, 0 , tmp_coll.getSize().x/1.5, 16));
	tmp_coll.setHealth(10);
	tmp_coll.setPosition(sf::Vector2f(window.getSize().x/2-tmp_coll.getSize().x/2, window.getSize().y/2-tmp_coll.getSize().y/2));
	tmp_coll.setHeight(1);
	tmp_coll.loadFromFile("data/entities/Tree.ent", &collectortext);

	map->addEntity(&tmp_coll);
	
	//Main loop
	while(window.isOpen())
	{
		window.clear();
			
		map->update(); // Update map


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
				window.setView(main_view);
			}
		}
		
		
		// Keyboard input are associated to the hero
		character_key_input(&personnage, &space);


		// Update attack state of the hero
		personnage.updateAttack();
		mob.updateAttack();

		// We apply physics on the objects associated with the current map
		map->physics_entities();

        // Characters updating
		personnage.update();
		mob.update();
		tmp_coll.update();


		// Drawing the map
		window.draw(*map);


		// Displaying the window
		window.display();
	}

	delete map;

	return 0;
}
