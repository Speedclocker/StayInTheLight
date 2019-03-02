#include <SFML/Graphics.hpp>
#include "Interactions.h"
#include "Character.h"
#include "collision.h"


int main()
{
        
	//Création de la fenêtre et paramétrage de framerate
	sf::RenderWindow window(sf::VideoMode(640,480), "Comment");
        window.setFramerateLimit(60);


	//Création et chargement de texture
        sf::Texture texture_link;
        texture_link.loadFromFile("imgs/linkmv.png");
    
        
        //Création du personnage
        Character personnage(&texture_link, sf::IntRect(0,0,30,30), sf::Color::Red);
        personnage.setSpeed(2);
        personnage.setHitbox(sf::IntRect(0,0, personnage.getSize().x, personnage.getSize().y));
        personnage.setHealth(10);
        personnage.setPosition(sf::Vector2f(window.getSize().x/2-personnage.getSize().x/2, window.getSize().y/2-personnage.getSize().y/2));

        
	//Création du mob
        Character mob(&texture_link, sf::IntRect(0,150,30,30), sf::Color::Blue);
	mob.setHealth(10);
        mob.setHitbox(sf::IntRect(0,0, mob.getSize().x, mob.getSize().y));
        mob.setPosition(sf::Vector2f(window.getSize().x/2-mob.getSize().x/2, window.getSize().y/2-100-mob.getSize().y/2));
        

	//Ajout d'une cible potentielle d'attaque au héros 
	personnage.addAvTarget(&mob);

        
   	//Variable d'état de la touche d'attaque
        bool space=false;

	//Les évenements ne se répètent pas tant que la ou les touches restent appuyées
        window.setKeyRepeatEnabled(false);
        

	//Boucle principale
	while(window.isOpen())
	{
		
		
                window.clear();
		
                
                // Attente des évenements
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
				space=true;
		}
		
		
		// Les entrées claviers permettent d'intéragir sur le personnage
		character_key_input(&personnage, &space);

                physics_characters(&personnage, &mob);
                
                
                // Mise à jour des personnages
		personnage.update();
		mob.update();
               

                
                
                
                
                // Dessin des personnages à l'écran
		mob.getDrawn(&window);
		personnage.getDrawn(&window);
		
		
		// Affichage des éléments
		window.display();
		
		
	}
	return 0;
}
