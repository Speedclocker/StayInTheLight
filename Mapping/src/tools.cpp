#include "tools.h"

#define FONT_FILE "../data/fonts/AldoTheApache.ttf"

#define ENTITIES_FOLDER "entities"
#define CHARACTER_SIZE_HEIGHT 20

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void list_entities(std::vector<std::string>* entities_files)
{
	entities_files->clear();

	DIR *d;
	struct dirent *dir;
	d = opendir(ENTITIES_FOLDER);
	if(d)
	{
		while((dir = readdir(d)) != NULL)
		{
			if(strstr(dir->d_name, ".ent\0"))
	        	entities_files->push_back(std::string(dir->d_name));
		}

        closedir(d);
	}
}


void control_view(sf::RenderWindow* window, sf::View* view, Map* map) 
{
	//Fonction qui permet de déplacer la vue avec le clic
	static bool clicked = false;
	static sf::Vector2f origin_click;

	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	sf::Vector2f refPos = origin_click;


	if (!clicked && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		origin_click = mousePos;
		view->setCenter((map->getSize().x) * map->getTileSize()/2, (map->getSize().y) * map->getTileSize()/2);
	}
	else if(!clicked && sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		origin_click = mousePos;
		clicked=true;
	}

	if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		refPos = sf::Vector2f((view->getCenter().x + origin_click.x - mousePos.x > 0 && view->getCenter().x + origin_click.x - mousePos.x > 0)? mousePos.x : refPos.x,
							  (view->getCenter().y + origin_click.y - mousePos.y > 0 && view->getCenter().y + origin_click.y - mousePos.y > 0)? mousePos.y : refPos.y);

		view->move( ( (view->getCenter().x + origin_click.x - refPos.x > 0) && (view->getCenter().x + origin_click.x - refPos.x < map->getSize().x * map->getTileSize()) )  ? origin_click.x - refPos.x : 0,
					( (view->getCenter().y + origin_click.y - refPos.y > 0) && (view->getCenter().y + origin_click.y - refPos.y < map->getSize().y * map->getTileSize()) )  ? origin_click.y - refPos.y : 0);
	}
	else
		clicked=false;
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



bool height_settings_command(sf::RenderWindow* window, Map* map, int* chosen_height)
{	
	static bool keyPressed_O = false, keyPressed_P = false, keyPressed_T = false, transparencydisplay=true;


	//Changement de hauteur -
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::O) && window->hasFocus())
		keyPressed_O = true;
	else if(keyPressed_O)
	{
		keyPressed_O = false;
		if(*chosen_height>0) (*chosen_height)--;
	}

	//Changement de hauteur +
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && window->hasFocus())
		keyPressed_P = true;
	else if(keyPressed_P)
	{
		keyPressed_P = false;
		if(*chosen_height < map->getHeight()-1 ) (*chosen_height)++;
	}

	//Mode transparent
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::T) && window->hasFocus())
		keyPressed_T = true;
	else if(keyPressed_T)
	{
		keyPressed_T = false;
		transparencydisplay^=true;
	}


	//Permet d'afficher la hauteur	
	char height_str[40];

	if(transparencydisplay)
		sprintf(height_str, "Hauteur : %d T-Mode", *chosen_height);
	else
		sprintf(height_str, "Hauteur : %d", *chosen_height);

	sf::Font height_font;
	height_font.loadFromFile(FONT_FILE);

	sf::Text height_text(height_str,height_font);
	height_text.setCharacterSize(CHARACTER_SIZE_HEIGHT);
	height_text.setFillColor(sf::Color::White);
	height_text.setOutlineColor(sf::Color::Black);
	height_text.setOutlineThickness(1);
	height_text.setPosition(window->getView().getCenter().x - window->getView().getSize().x/2 + 10, window->getView().getCenter().y + window->getView().getSize().y/2 - 30);

	window->draw(height_text);


	return transparencydisplay;
}


void save_map_command(Map* map_to_save, std::string texture_name, Tile* tileset, int tileset_size)
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

		if(saveMap(map_to_save, map_name, file_name, texture_name, tileset, tileset_size)<0)
		{
			std::cerr << "Une erreur a eu lieu durant la sauvegarde de la map..." << std::endl;
		}
		else
		{
			std::cout << "La map a été sauvegardée avec succès !" << std::endl;
		}
	}
}


int load_map_command(sf::RenderWindow* main_window, Map** map_to_load, sf::Texture* texture, Tile** tileset, int* tileset_size, bool fullscreen)
{
	// Pour le chargement de la map
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		std::string map_name;
		std::cout << "Chargement : quel est le nom du fichier map : " << std::endl;
		std::cin >> map_name;

		std::string tmp_text_file;

		// Chargement de la map
		if(loadMap(map_to_load, map_name, &tmp_text_file, texture, tileset, tileset_size)<0)
		{ 
			std::cerr << "Une erreur a eu lieu lors du chargement de la map." << std::endl; 
			return -1;
		}
		else
		{
			// Modification de la texture
			if( texture->loadFromFile(tmp_text_file) == false) { std::cerr << "Le fichier texture n'a pas pu être chargé." << std::endl; }
			else (*map_to_load)->setTexture(texture);

			if(!fullscreen)
			{
				//Modification de la fenêtre principale
				main_window->setSize(sf::Vector2u(( (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x<1200) ? (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x : 1200 
									,( (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y<720)  ? (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y : 720));

			}

			//Modification de la vue
			sf::View main_view(sf::Vector2f((*map_to_load)->getTileSize() * (*map_to_load)->getSize().x/2, (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y/2), sf::Vector2f(main_window->getSize()));
			main_window->setView(main_view);
		
/*
			//Modification des tiles dispo:
			*nbr_tiles=(texture->getSize().x/(*map_to_load)->getTileSize())*(texture->getSize().y/(*map_to_load)->getTileSize());

			*available_tiles= (Tile*)realloc((*available_tiles),(*nbr_tiles)*sizeof(Tile));

			for(int i=0; i<*nbr_tiles;i++)
			{
				(*available_tiles)[i].m_pos_text = sf::Vector2f( (i%(texture->getSize().x/(*map_to_load)->getTileSize()))*(*map_to_load)->getTileSize(),  (i/(texture->getSize().x/(*map_to_load)->getTileSize()))*(*map_to_load)->getTileSize() );
				(*available_tiles)[i].m_size_text = sf::Vector2f( (*map_to_load)->getTileSize(), (*map_to_load)->getTileSize() );
				(*available_tiles)[i].m_collisionable = false;
			}
*/
			return 1;
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void introText(void)
{
	std::cout << "\t\t=== MapCreator ===\n\n" << std::endl;	
	std::cout << "MapCreator permet de créer ses propres maps" << std::endl;
	std::cout << "Sélectionnez les tiles sur la fenêtre de tileset" << std::endl;
	std::cout << "Pour voir les couches inférieurs aux couches supérieures, passez en mode transparent" << std::endl;
	std::cout << "Pour celà, appuyez sur T" << std::endl;
	std::cout << "Pour gérer les collisions de chaque tile au niveau du tileset, maintenez la touche LAlt et changez l'état du tile que vous souhaitez en cliquant dessus" << std::endl;
	std::cout << "Pour poser plusieurs tiles, maintenez LShift et faites glisser le curseur" << std::endl;
	std::cout << "Pour sauvegarder la map, maintenez LAlt et appuyez S. Les maps sont enregistrées dans le dossier ./maps" << std::endl;
	std::cout << "Pour charger une map, maintenez LAlt et appuyez L" << std::endl;
}

