#include <SFML/Graphics.hpp>
#include <math.h>
#include "Character.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(640,480), "Comment");
        sf::Texture texture_link;
        texture_link.loadFromFile("link.png");
        
        
        Character personnage(&texture_link, sf::IntRect(0,0,30,30));
        personnage.setSpeed(3);

        window.setFramerateLimit(60);

        personnage.setPosition(sf::Vector2f(window.getSize().x/2-personnage.getSize().x/2, window.getSize().y/2-personnage.getSize().y/2));
	
        
	while(window.isOpen())
	{
                window.clear();
                
                
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}
		
		
		
		
		// Déplacement du personnage
		
		int speed = personnage.getSpeed();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    personnage.move(sf::Vector2f(+sqrt(speed), -sqrt(speed)));
                
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    personnage.move(sf::Vector2f(-sqrt(speed), -sqrt(speed)));
                
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    personnage.move(sf::Vector2f(+sqrt(speed), +sqrt(speed)));
                
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    personnage.move(sf::Vector2f(-sqrt(speed), +sqrt(speed)));
                
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                    personnage.move(sf::Vector2f(0, -speed));
                
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    personnage.move(sf::Vector2f(0, speed));
                
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                    personnage.move(sf::Vector2f(-speed, 0));
                
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    personnage.move(sf::Vector2f(speed, 0));
		
                
                
                
                
                
		
		personnage.getDrawn(&window);
		window.display();
	}
	return 0;
}
