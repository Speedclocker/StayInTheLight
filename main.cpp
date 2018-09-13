#include <SFML/Graphics.hpp>
#include <math.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(640,480), "Comment");
        sf::Texture texture_link;
        texture_link.loadFromFile("link.png");
        sf::Sprite personnage(texture_link, sf::IntRect(0,0,30, 30));
        int speed = 2;

        window.setFramerateLimit(60);

        personnage.setPosition(sf::Vector2f(window.getSize().x/2-personnage.getTextureRect().width/2, window.getSize().y/2-personnage.getTextureRect().height/2));
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}
		
		
		
		
		// Déplacement du personnage
		
		
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
		
                
                
                
                
                
		window.clear();
		window.draw(personnage);
		window.display();
	}
	return 0;
}
