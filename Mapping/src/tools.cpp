#include "tools.h"

#define FONT_FILE "../data/fonts/monaco.ttf"

#define ENTITIES_FOLDER "entities"
#define CHARACTER_SIZE_HEIGHT 26

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


void saveMapCommand(Map* map_to_save, std::vector<Entity*> entities_to_save, std::string texture_name, Tile* tileset, int tileset_size)
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

		if(saveMap(map_to_save, entities_to_save, map_name, file_name, texture_name, tileset, tileset_size)<0)
		{
			std::cerr << "Une erreur a eu lieu durant la sauvegarde de la map..." << std::endl;
		}
		else
		{
			std::cout << "La map a été sauvegardée avec succès !" << std::endl;
		}
	}
}


int loadMapCommand(sf::RenderWindow* main_window, Map** map_to_load, std::vector<Entity*>* entities_to_load, sf::Texture** texture, Tile** tileset, int* tileset_size, ResourcesManager* resources_manager, bool fullscreen)
{
	// Pour le chargement de la map
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		std::string map_name;
		std::cout << "Chargement : quel est le nom du fichier map : " << std::endl;
		std::cin >> map_name;

		std::string tmp_text_file;

		// Chargement de la map
		if(loadMap(map_to_load, entities_to_load, map_name, &tmp_text_file, texture, tileset, tileset_size, resources_manager)<0)
		{ 
			std::cerr << "Une erreur a eu lieu lors du chargement de la map." << std::endl; 
			return -1;
		}
		else
		{
			// Modification de la texture
			//if( texture->loadFromFile(tmp_text_file) == false) { std::cerr << "Le fichier texture n'a pas pu être chargé." << std::endl; }
			(*map_to_load)->setTexture(*texture);

			if(!fullscreen)
			{
				//Modification de la fenêtre principale
				main_window->setSize(sf::Vector2u(( (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x<1200) ? (*map_to_load)->getTileSize() * (*map_to_load)->getSize().x : 1200 
									,( (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y<720)  ? (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y : 720));

			}

			//Modification de la vue
			sf::View main_view(sf::Vector2f((*map_to_load)->getTileSize() * (*map_to_load)->getSize().x/2, (*map_to_load)->getTileSize() * (*map_to_load)->getSize().y/2), sf::Vector2f(main_window->getSize()));
			main_window->setView(main_view);


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

