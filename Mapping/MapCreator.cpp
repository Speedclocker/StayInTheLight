#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Map.h"
#include "saveMap.h"

void choice_tile(Tile* tile_target, sf::RenderWindow* window, sf::Texture* texture_file, Tile tiles[], int nbr_avail_tiles, int size_tile)
{
	//Fonction qui permet le choix du tile
	static sf::RectangleShape choice_square(tiles[0].m_size_text);
	choice_square.setFillColor(sf::Color(0,0,0,0));
	choice_square.setOutlineColor(sf::Color(255,0,0,255));
	choice_square.setOutlineThickness(2);

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	//Fonction qui renvoie le tile choisi
	if(mousePos.x >=0 && mousePos.x < texture_file->getSize().x
	&& mousePos.y >=0 && mousePos.y < texture_file->getSize().y )
	{

		sf::Vector2f positionTile((int)(mousePos.x/size_tile)*size_tile, (int)(mousePos.y/size_tile)*size_tile);

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->hasFocus())
		{
			int index=(int)(positionTile.x/size_tile) + (int)(positionTile.y/size_tile) * (int)(texture_file->getSize().x/size_tile) ;
			choice_square.setPosition(positionTile);

			if(index<nbr_avail_tiles) 
			{
				*tile_target=tiles[index];
				std::cout << "Mouse Position : " << positionTile.x << " , " << positionTile.y << std::endl;
				std::cout << "Index : " << index << std::endl;

			}
		}

	}

	window->draw(choice_square);
}

/*

void hitbox_settings(sf::RenderWindow* window, Map map, std::vector< sf::IntRect >* hitbox_zones)
{
	static bool click=false;
	static sf::Vector2f origin_click(0,0), corner_click(0,0);




	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && window->hasFocus())
	{
		sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
		{
			origin_click=mousePos;
			click=true;
		}
		else if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			corner_click=mousePos;
		}
	}

}
*/



void tileset_collision_settings(sf::RenderWindow* window, sf::Texture* texture_file, Tile tiles[], int nbr_avail_tiles, int size_tile)
{
	static bool clicked = false;
	sf::VertexArray state_collision_bloc(sf::Quads, 4*nbr_avail_tiles);
	sf::Texture state_text;
	state_text.loadFromFile("imgs/state_collision.png");


	//Affiche la collisionnabilité des tiles
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));


		if(mousePos.x >=0 && mousePos.x < texture_file->getSize().x
		&& mousePos.y >=0 && mousePos.y < texture_file->getSize().y )
		{

			
			if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
				clicked=true;

			else if(clicked)
			{
				clicked=false;
				sf::Vector2f positionTile((int)(mousePos.x/size_tile)*size_tile, (int)(mousePos.y/size_tile)*size_tile);

				int index=(int)(positionTile.x/size_tile) + (int)(positionTile.y/size_tile) * (int)(texture_file->getSize().x/size_tile) ;

				tiles[index].m_collisionable^=true;
			}
			

		}

		//Montre l'état de collision des tiles
		for(int i=0; i<nbr_avail_tiles; i++)
		{
			state_collision_bloc[i*4].position		=	tiles[i].m_pos_text;
			state_collision_bloc[i*4+1].position 	= 	tiles[i].m_pos_text + sf::Vector2f(tiles[i].m_size_text.x, 0);
			state_collision_bloc[i*4+2].position 	= 	tiles[i].m_pos_text + sf::Vector2f(tiles[i].m_size_text.x, tiles[i].m_size_text.y);
			state_collision_bloc[i*4+3].position 	= 	tiles[i].m_pos_text	 + sf::Vector2f(0, tiles[i].m_size_text.y);

			sf::Vector2f offset_texture( (tiles[i].m_collisionable) ? state_text.getSize().x/2 : 0 , 0);



			state_collision_bloc[i*4].texCoords		=	sf::Vector2f(0, 0) + offset_texture;
			state_collision_bloc[i*4+1].texCoords 	= 	sf::Vector2f(state_text.getSize().x/2, 0) + offset_texture;
			state_collision_bloc[i*4+2].texCoords 	= 	sf::Vector2f(state_text.getSize().x/2, state_text.getSize().y) + offset_texture;
			state_collision_bloc[i*4+3].texCoords 	= 	sf::Vector2f(0, state_text.getSize().y) + offset_texture;
		}

		window->draw(state_collision_bloc, &state_text);

	}

}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void control_view(sf::View* view, Map* map) 
{
	//Fonction permettant de contrôler la vue
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (view->getCenter().y - view->getSize().y/2 > 0) ) 
	{
		view->move(0, -10);
		std::cout << "View : " << view->getCenter().x << " , " << view->getCenter().y << std::endl;
	} 
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (view->getCenter().y + view->getSize().y/2 < map->getSize().y * map->getTileSize() ) ) 
	{
		view->move(0, 10);		
		std::cout << "View : " << view->getCenter().x << " , " << view->getCenter().y << std::endl;
	}
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (view->getCenter().x - view->getSize().x/2 > 0) ) 
	{
		view->move(-10, 0);
		std::cout << "View : " << view->getCenter().x << " , " << view->getCenter().y << std::endl;
	}
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (view->getCenter().x + view->getSize().x/2 < map->getSize().x * map->getTileSize()) ) 
	{
		view->move(10, 0);
		std::cout << "View : " << view->getCenter().x << " , " << view->getCenter().y << std::endl;
	}	
}



void set_tile(sf::RenderWindow* window, sf::Texture *texture, Map* map, Tile tile, int height)
{
	// Fonction qui gère la pose des blocs dans la fenêtre de mapping

	static bool square_clicked=false;
	static sf::Vector2f origin_click(0,0), corner_click(0,0);

	sf::Sprite tmprect(*texture, sf::IntRect((sf::Vector2i)tile.m_pos_text, (sf::Vector2i)tile.m_size_text));
	tmprect.setColor(sf::Color(255,255,255,100));

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));




	if(mousePos.x >=0 && mousePos.x < map->getSize().x*map->getTileSize()
	&& mousePos.y >=0 && mousePos.y < map->getSize().y*map->getTileSize() )
	{

		sf::Vector2f positionTile((int)(mousePos.x/map->getTileSize())*map->getTileSize(), (int)(mousePos.y/map->getTileSize())*map->getTileSize());
		tmprect.setPosition(positionTile);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && window->hasFocus())
		{
			// Pose de plusieurs blocs
			if(!square_clicked)
			{
				square_clicked=true;
			}
			
			corner_click=positionTile;

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for(int i=origin_click.x; (origin_click.x<=corner_click.x)? i <= corner_click.x : i >= corner_click.x; (origin_click.x<=corner_click.x)? i+=tile.m_size_text.x : i-=tile.m_size_text.x)
				{
					for(int j=origin_click.y; (origin_click.y<=corner_click.y)? j <= corner_click.y : j >= corner_click.y; (origin_click.y<=corner_click.y)? j+=tile.m_size_text.y : j-=tile.m_size_text.y)
					{
						map->setTile(tile, sf::Vector2i(i, j)/map->getTileSize(), height);
					}
				}

				origin_click=positionTile;
			}
		}
		else 
		{
			square_clicked=false;

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->hasFocus())
			{
				origin_click=positionTile;
				map->setTile(tile, (sf::Vector2i)positionTile/map->getTileSize(), height);
			}
		}


		window->draw(tmprect);
	}


	// Affichage des blocs fantomes en cas de selection multiple
	if(square_clicked)
	{
		for(int i=origin_click.x; (origin_click.x<=corner_click.x)? i <= corner_click.x : i >= corner_click.x; (origin_click.x<=corner_click.x)? i+=tile.m_size_text.x : i-=tile.m_size_text.x)
		{
			for(int j=origin_click.y; (origin_click.y<=corner_click.y)? j <= corner_click.y : j >= corner_click.y; (origin_click.y<=corner_click.y)? j+=tile.m_size_text.y : j-=tile.m_size_text.y)
			{
				tmprect.setPosition(sf::Vector2f(i, j));
				window->draw(tmprect);

			}
		}
	}
}



bool height_settings(sf::RenderWindow* window, Map* map, int* chosen_height)
{	
	static bool keyPressed_O = false, keyPressed_P = false, keyPressed_T = false, transparencydisplay=false;

	//Changement de hauteur -
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::O) && window->hasFocus())
		keyPressed_O = true;
	else if(keyPressed_O)
	{
		keyPressed_O = false;
		if(*chosen_height>0) (*chosen_height)--;
		std::cout << "Hauteur : " << *chosen_height << std::endl;
	}

	//Changement de hauteur +
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && window->hasFocus())
		keyPressed_P = true;
	else if(keyPressed_P)
	{
		keyPressed_P = false;
		if(*chosen_height < map->getHeight()-1 ) (*chosen_height)++;
		std::cout << "Hauteur : " << *chosen_height << std::endl;
	}

	//Mode transparent
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T) && window->hasFocus())
		keyPressed_T = true;
	else if(keyPressed_T)
	{
		keyPressed_T = false;
		transparencydisplay^=true;
	}

	return transparencydisplay;
}


void save_map(Map* map_to_save, std::string texture_name)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		std::string map_name;
		std::cout << "Sauvegarde : donnez un nom à la map : " << std::endl;
		std::cin >> map_name;

		std::string file_name=map_name;
		std::size_t tmp_space;
		while( (tmp_space=file_name.find(" "))!= std::string::npos)
			file_name.erase(tmp_space,1);
		file_name=file_name+".map";

		if(saveMap(map_to_save, map_name, file_name, texture_name)<0)
		{
			std::cerr << "Une erreur a eu lieu durant la sauvegarde de la map..." << std::endl;
		}
		else
		{
			std::cout << "La map a été sauvegardée avec succès !" << std::endl;
		}
	}
}


int load_map(sf::RenderWindow* main_window, sf::RenderWindow* tileset_window, Map** map_to_load, sf::Texture* texture, Tile** available_tiles, int* nbr_tiles)
{
	// Pour le chargement de la map
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		std::string map_name;
		std::cout << "Chargement : quel est le nom du fichier map : " << std::endl;
		std::cin >> map_name;

		std::string tmp_text_file;

		// Chargement de la map
		if(loadMap(map_to_load, map_name, &tmp_text_file)<0) 
		{ 
			std::cerr << "Une erreur a eu lieu lors du chargement de la map." << std::endl; 
			return -1;
		}
		else
		{
			// Modification de la texture
			texture->loadFromFile(tmp_text_file);

			if( texture->loadFromFile(tmp_text_file) == false) { std::cerr << "Le fichier texture n'a pas pu être chargé." << std::endl; }
			else (*map_to_load)->setTexture(texture);


			//Modification de la fenêtre principale
			main_window->setSize(sf::Vector2u(( (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x<1200) ? (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x : 1200 
								,( (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y<720)  ? (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y : 720));


			//Modification de la vue
			sf::View main_view(sf::Vector2f((*map_to_load)->getTileSize() * (*map_to_load)->getSize().x/2, (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y/2), sf::Vector2f(main_window->getSize()));
			std::cout << (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x/2 << " // " << (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x/2 << std::endl;
			main_window->setView(main_view);


			//Modification de la fenêtre de tileset
			tileset_window->setSize(texture->getSize());
			sf::Sprite tileset_sprite(*texture);
			tileset_sprite.setPosition(0,0);

			//Modification des tiles dispo:
			*nbr_tiles=(texture->getSize().x/(*map_to_load)->getTileSize())*(texture->getSize().y/(*map_to_load)->getTileSize());

			*available_tiles= (Tile*)realloc((*available_tiles),(*nbr_tiles)*sizeof(Tile));

			for(int i=0; i<*nbr_tiles;i++)
			{
				(*available_tiles)[i].m_pos_text = sf::Vector2f( (i%(texture->getSize().x/(*map_to_load)->getTileSize()))*(*map_to_load)->getTileSize(),  (i/(texture->getSize().x/(*map_to_load)->getTileSize()))*(*map_to_load)->getTileSize() );
				(*available_tiles)[i].m_size_text = sf::Vector2f( (*map_to_load)->getTileSize(), (*map_to_load)->getTileSize() );
				(*available_tiles)[i].m_collisionable = false;
				std::cout << "Tiles " << i << " : " << (*available_tiles)[i].m_pos_text.x << " , " << (*available_tiles)[i].m_pos_text.y << std::endl;
			}
			return 1;
		}
	}
	return 0;
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





	std::cout << "\t\t=== MapCreator ===\n\n" << std::endl;	

	//Création de la fenêtre principale
	sf::RenderWindow window(sf::VideoMode((size_tile*size_x<1200) ? size_tile*size_x : 1200 , (size_tile*size_y<720) ? size_tile*size_y : 720), "MapCreator");
	window.setFramerateLimit(120);


	//Création de la fenêtre adjacente
	sf::RenderWindow tileset_window(sf::VideoMode(texture_file.getSize().x,texture_file.getSize().y), "Tileset", sf::Style::Titlebar);
	sf::Sprite tileset_sprite(texture_file);
	tileset_sprite.setPosition(0,0);


	//Initialisation des tiles
	int nbr_tiles=(texture_file.getSize().x/size_tile)*(texture_file.getSize().y/size_tile);
	Tile* available_tiles= (Tile*)malloc(nbr_tiles * sizeof(Tile));
	for(int i=0; i<nbr_tiles;i++)
	{
		available_tiles[i].m_pos_text = sf::Vector2f( (i%(texture_file.getSize().x/size_tile))*size_tile,  (i/(texture_file.getSize().x/size_tile))*size_tile );
		available_tiles[i].m_size_text = sf::Vector2f( size_tile, size_tile );
		available_tiles[i].m_collisionable = false;
		std::cout << "Tiles " << i << " : " << available_tiles[i].m_pos_text.x << " , " << available_tiles[i].m_pos_text.y << std::endl;
	}
	Tile buff_tile=available_tiles[0];


	//Création de la vue
	sf::View main_view(sf::Vector2f(size_x*size_tile/2, size_y*size_tile/2), sf::Vector2f(window.getSize()));
	window.setView(main_view);


/*
	Map* custom_map=NULL;
	std::string tmp_text_file;
	tmp_text_file.erase(std::remove(tmp_text_file.begin(), tmp_text_file.end(), ' '), tmp_text_file.end());

	loadMap(&custom_map, "LittleHouse.map", &tmp_text_file);

	std::cout << tmp_text_file << std::endl;

	sf::Texture new_text;
	
	if( new_text.loadFromFile(tmp_text_file) == false)
	{
		std::cout << "Le fichier texture n'a pas pu être chargé." << std::endl;
		return -1;
	}
	custom_map->setTexture(&new_text);

*/


	//Création de la map

	Map* custom_map=new Map(sf::Vector2f(size_x, size_y), size_h, size_tile);
	custom_map->setTexture(&texture_file);

	//Zone Map
	sf::RectangleShape map_zone(sf::Vector2f(custom_map->getSize().x*custom_map->getTileSize(), custom_map->getSize().y*custom_map->getTileSize()));
	map_zone.setFillColor(sf::Color(0,0,0,0));
	map_zone.setOutlineThickness(2);
	map_zone.setOutlineColor(sf::Color::Red);
	int chosen_height=0;


	//Boucle principale
	while(window.isOpen() && tileset_window.isOpen())
	{
		map_zone.setSize(sf::Vector2f(custom_map->getSize().x*custom_map->getTileSize(), custom_map->getSize().y*custom_map->getTileSize()));
		window.clear();
		tileset_window.clear();

        // Attente des évenements
		sf::Event event, tileset_event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
				tileset_window.close();
			}
			
			if (event.type == sf::Event::Resized)
			{
				main_view.setSize(event.size.width, event.size.height);
				main_view.setCenter(sf::Vector2f(custom_map->getTileSize() * custom_map->getSize().x/2, custom_map->getTileSize() * custom_map->getSize().y/2));
				window.setView(main_view);
			}


		}

		while(tileset_window.pollEvent(tileset_event))
		{

		}
		

		// Changement de hauteur
		if(height_settings(&window, custom_map, &chosen_height))
			custom_map->update_transparency(chosen_height);
		else
			custom_map->update();
		

		window.draw(*custom_map);
		window.draw(map_zone);

		//Si l'utilisateur utilise la fenêtre principale
		if(window.hasFocus())
		{
			//Contrôle de la vue
			control_view(&main_view, custom_map);
			window.setView(main_view);
		}


		//Pour placer les tiles
		set_tile(&window, &texture_file, custom_map, buff_tile, chosen_height);

		//Pour sauvegarder la map
		save_map(custom_map, texture_file_name);

		//Pour charger une map
		if(load_map(&window, &tileset_window, &custom_map, &texture_file, &available_tiles, &nbr_tiles)==1)
		{
			buff_tile=available_tiles[0];
			std::cout << "bouffon : " << map_zone.getPosition().x << std::endl;
		}

		//Placement des élements
		tileset_window.draw(tileset_sprite);


		//Choix de la tile
		choice_tile(&buff_tile, &tileset_window, &texture_file, available_tiles, nbr_tiles, custom_map->getTileSize());

		if(tileset_window.hasFocus())
		{
			//Gestion de la collision des blocs
			tileset_collision_settings(&tileset_window, &texture_file, available_tiles, nbr_tiles, custom_map->getTileSize());	
		}
	
		// Affichage des éléments
		window.display();
		tileset_window.display();
		
	}

	delete custom_map;

	free(available_tiles);

	return 0;

}