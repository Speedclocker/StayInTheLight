#include "saveMap.h"



void probeMap(const char* raw_line, Map* load_location_map)
{
	static int actual_height=0, actual_X=0;
	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;
	if(strstr(line, "H")!=NULL && strstr(line, "/H")==NULL)
	{
		//Detection d'un changement de hauteur
		buffer = strtok(line, "H:\n");
		

		actual_height = atoi(buffer);
		actual_X = 0;

	}		
	else if(strstr(line, "|")!=NULL && (strstr(line, "|") - line)==0 )
	{
		//Detection de | en début de ligne
		
		std::vector<std::string> tmptile;
		buffer=strtok(line, "(");
		buffer=strtok(NULL, ")");

		if(buffer!=NULL)
		{
			std::string tmp_str=buffer;
			tmptile.push_back(tmp_str);
		}	
		
		while(buffer!=NULL)
		{
			buffer=strtok(NULL, "(");
			buffer=strtok(NULL, ")\n");

			if(buffer!=NULL)
			{
				std::string tmp_str=buffer;
				tmptile.push_back(tmp_str);
			}			
		}

		int actual_Y=0;
		for (std::vector<std::string>::iterator it = tmptile.begin() ; it != tmptile.end(); it++)
		{
			Tile tile;
			buffer=strtok((char*)((*it).c_str()), ",");
			tile.m_pos_text.x = atoi(buffer);

			buffer=strtok(NULL, "-");
			tile.m_pos_text.y = atoi(buffer);

			buffer=strtok(NULL, "\n");
			tile.m_collisionable = atoi(buffer);

			tile.m_size_text=sf::Vector2f(load_location_map->getTileSize(), load_location_map->getTileSize());

			load_location_map->setTile(tile, sf::Vector2i(actual_X , actual_Y), actual_height);

			actual_Y++;
		}

		actual_X++;
	}


}

void probeHeader(const char* raw_line, sf::Vector2f* size_map, int* height_map, int* tile_sz_map, char texture_name_file[BUFF_SIZE])
{
	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;

	if(strstr(line, "Name : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		std::cout << "Nom de la map : " << buffer << std::endl;
	}
	else if(strstr(line, "Tile Size : ")!=NULL)
	{	
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		*tile_sz_map = atoi(buffer);
		std::cout << "Taille du tile : " << *tile_sz_map << std::endl;
	}
	else if(strstr(line, "Height : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		*height_map = atoi(buffer);
		std::cout << "Hauteur de la map : " << *height_map << std::endl;
	}
	else if(strstr(line, "X : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		size_map->x = atoi(buffer);
		std::cout << "Taille en x : " << size_map->x << std::endl;
	}
	else if(strstr(line, "Y : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		size_map->y = atoi(buffer);
		std::cout << "Taille en y : " << size_map->y << std::endl;
	}
	else if(strstr(line, "Texture File : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n") + 1;
		strcpy(texture_name_file, buffer);
		std::cout << "Texture file : " << texture_name_file << std::endl;
	}
	
}


int loadMap(Map** load_location_map, std::string name_file_map_to_load, std::string* name_texture_file)
{
	std::ifstream file_to_load;
	file_to_load.open("maps/"+name_file_map_to_load);
	if(file_to_load.fail()) {std::cerr << "Une erreur est survenue lors de la tentative d'ouverture du fichier " << name_file_map_to_load << std::endl; file_to_load.close(); return -1; }

	sf::Vector2f size_map=sf::Vector2f(-1,-1);
	int height_map=-1, tile_sz_map=-1;
	char texture_name_file[BUFF_SIZE];

	if(*load_location_map != NULL)
	{
		delete *load_location_map;
		*load_location_map=NULL;
	}

	std::string buffer="";

	//Lecture du fichier
	while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "Map :")==NULL)
	{
		probeHeader(buffer.c_str(), &size_map, &height_map, &tile_sz_map, texture_name_file);
	}

	if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }

	*load_location_map = new Map(size_map, height_map, tile_sz_map);
	*name_texture_file = texture_name_file;

	while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "/Map")==NULL)
	{
		probeMap(buffer.c_str(), *load_location_map);
	}
	file_to_load.close();
	std::cout << "end" <<std::endl;

	return 0;
}



int saveMap(Map* map_to_save, std::string name_map, std::string name_file, std::string texture_name_file)
{
	std::ofstream file_to_save;

	file_to_save.open("maps/"+name_file);
	if(file_to_save.fail()) {std::cerr << "Une erreur est survenue lors de la tentative de création du fichier " << name_file << std::endl; file_to_save.close(); return -1; }

	//Enregistrement du nom 
	file_to_save << "Name : " << name_map <<std::endl;

	//Enregistrement de la taille d'une size
	file_to_save << "Tile Size : " << map_to_save->getTileSize() << std::endl;

	//Enregistrement de la Hauteur de la map
	file_to_save << "Height : " << map_to_save->getHeight() << std::endl;

	//Enregistrement de la taille de la map
	file_to_save << "X : " << map_to_save->getSize().x << std::endl;
	file_to_save << "Y : " << map_to_save->getSize().y << std::endl;

	//Enregistrement du nom du fichier de texture
	file_to_save << "Texture File : " << texture_name_file << std::endl;

	file_to_save << std::endl;


	//Enregistrement de la map
	file_to_save << "Map : " << std::endl;
	for(int h=0; h<map_to_save->getHeight(); h++)
	{
		file_to_save << "H" << h << ": " << std::endl;
		for (int i = 0; i < map_to_save->getSize().x; i++)
		{
			file_to_save << "| " ;
			for (int j = 0; j < map_to_save->getSize().y; j++)
			{
				Tile tmp_tile = map_to_save->getTile(h, sf::Vector2i(i, j));

				if(tmp_tile.m_pos_text.x < 0 || tmp_tile.m_pos_text.y < 0) 
				{ 
					std::cerr << "Une erreur a eu lieu lors de l'enregistrement de la map." << std::endl;
					file_to_save.close(); 
					remove(name_file.c_str());
					return -1; 
				}
				
				file_to_save << "(" << tmp_tile.m_pos_text.x << "," << tmp_tile.m_pos_text.y << "-" << tmp_tile.m_collisionable << ") " ;
				
			}
			file_to_save << "|" << std::endl ;
		}
		file_to_save << "/H" << h << std::endl;
	}
	file_to_save << "/Map" << std::endl;

	file_to_save.close();

	return 0;
}