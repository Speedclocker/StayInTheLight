#include <SFML/Graphics.hpp>
#include <ctime>


#include "Interactions.h"
#include "Character.h"
#include "collision.h"
#include "loadMap.h"

int main()
{

	//Création de la fenêtre et paramétrage de framerate
	sf::RenderWindow window(sf::VideoMode(640,480), "Comment");
	window.setFramerateLimit(60);



   	//Variable d'état de la touche d'attaque
	bool space=false;

	//Les évenements ne se répètent pas tant que la ou les touches restent appuyées
	window.setKeyRepeatEnabled(false);


	//Création de la map
	Map* map=NULL;

	sf::Texture texture_map;
	std::string texture_file;

	if(loadMap(&map, "data/maps/Caca.map", &texture_file)<0)
	{
		std::cerr << "Erreur lors du chargement de la map" << std::endl;
		return -1;
	}
	texture_map.loadFromFile(texture_file);

	map->setTexture(&texture_map);



	//Création et chargement de texture
	sf::Texture texture_link;
	texture_link.loadFromFile("data/imgs/linkmv.png");


	//Création du personnage
	/*personnage.getSize().y-10*/
	Character personnage(&texture_link, sf::IntRect(0,0,30,30), sf::Color::Red);
	personnage.setSpeed(2);
	personnage.setHitbox(sf::IntRect(0, 5 , personnage.getSize().x/1.5, 16));
	personnage.setHealth(10);
	personnage.setPosition(sf::Vector2f(window.getSize().x/2-personnage.getSize().x/2, window.getSize().y/2-personnage.getSize().y/2));
	personnage.setHeight(1);
	map->addObject(&personnage);

	//Création du mob
	Character mob(&texture_link, sf::IntRect(0,150,30,30), sf::Color::Blue);
	mob.setHealth(10);
	mob.setHitbox(sf::IntRect(0, 5 , mob.getSize().x/1.5, 16 /*mob.getSize().y-10*/));
	mob.setPosition(sf::Vector2f(window.getSize().x/2-mob.getSize().x/2, window.getSize().y/2-100-mob.getSize().y/2));
	mob.setHeight(1);
	map->addObject(&mob);


	//Ajout d'une cible potentielle d'attaque au héros 
	personnage.addAvTarget(&mob);



	
	
	//Boucle principale
	while(window.isOpen())
	{
		window.clear();
			
		//Mise à jour de la map
		map->update();


        // Attente des évenements
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed || ( (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) )
				window.close();
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
				space=true;
		}
		
		
		// Les entrées claviers permettent d'intéragir sur le personnage
		character_key_input(&personnage, &space);


		// Mise à jour de l'attaque des personnages
		personnage.updateAttack();
		mob.updateAttack();


		// On applique la physique sur les objets liés à la map
		map->physics_objects();


        // Mise à jour des personnages
		personnage.update();
		mob.update();


		// Dessin de la map
		window.draw(*map);


        // Dessin des personnages à l'écran
        //window.draw(mob);
        //window.draw(personnage);


		// Affichage des éléments
		window.display();
	}

	delete map;

	return 0;
}
