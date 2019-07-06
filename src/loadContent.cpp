#include "loadContent.h"


/*-------------------------------------------- Load Map -----------------------------------------------------*/


void probeMap(const char* raw_line, Map* load_location_map)
{
	// Probing Map tag
	static int actual_height=0, actual_X=0;
	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;

	if(strstr(line, "[H")!=NULL)
	{
		// If the actual level of height is changed
		buffer = strtok(line, "H\n");
		buffer = strtok(NULL, "]\n");

		actual_height = atoi(buffer);
		actual_X = 0;

	}		
	else if(strstr(line, "|")!=NULL && (strstr(line, "|") - line)==0 )
	{
		// | char detection at the start of the line
		
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
			tile.m_pos_text.x = atoi(buffer)*load_location_map->getTileSize();

			buffer=strtok(NULL, "-");
			tile.m_pos_text.y = atoi(buffer)*load_location_map->getTileSize();

			buffer=strtok(NULL, "\n");

			tile.m_collisionable = atoi(buffer);

			tile.m_size_text=sf::Vector2f(load_location_map->getTileSize(), load_location_map->getTileSize());

			load_location_map->setTile(tile, sf::Vector2i(actual_X , actual_Y), actual_height);

			actual_Y++;
		}

		actual_X++;
	}
}


void probeHeader(const char* raw_line, sf::Vector2f* size_map, int* height_map, int* tile_sz_map, char texture_file_name[BUFF_SIZE])
{
	// Probing Header tag

	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;

	if(strstr(line, "Name : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
	}
	else if(strstr(line, "Tile Size : ")!=NULL)
	{	
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		*tile_sz_map = atoi(buffer);
	}
	else if(strstr(line, "Height : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		*height_map = atoi(buffer);
	}
	else if(strstr(line, "X : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		size_map->x = atoi(buffer);
	}
	else if(strstr(line, "Y : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n");
		size_map->y = atoi(buffer);
	}
	else if(strstr(line, "Texture File : ")!=NULL)
	{
		strtok(line, ":\n");
		buffer = strtok(NULL, ":\n") + 1;
		strcpy(texture_file_name, buffer);
	}
	
}


int loadMap(Map** load_location_map, std::string map_to_load_file_name, std::string* name_texture_file)
{
	// Load the map 

	std::ifstream file_to_load;
	file_to_load.open(map_to_load_file_name);
	if(file_to_load.fail()) {std::cerr << "Une erreur est survenue lors de la tentative d'ouverture du fichier " << map_to_load_file_name << std::endl; file_to_load.close(); return -1; }

	sf::Vector2f size_map=sf::Vector2f(-1,-1);
	int height_map=-1, tile_sz_map=-1;
	char texture_file_name[BUFF_SIZE];

	std::string buffer="";

	while(std::getline(file_to_load, buffer))
	{
		// Read the file until we reach the end
		if(strstr(buffer.c_str(), "[Header]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Header]")==NULL)
			{
				probeHeader(buffer.c_str(), &size_map, &height_map, &tile_sz_map, texture_file_name);
			}

			*name_texture_file = texture_file_name;

			//If an error is met
			if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }
			
			// If no error during map loading, we free the map
			if(*load_location_map != NULL)
			{
				delete *load_location_map;
				*load_location_map=NULL;
			}


			// Creation of map
			*load_location_map = new Map(size_map, height_map, tile_sz_map);
		}

		else if(strstr(buffer.c_str(), "[Map]")!=NULL)
		{
			if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }

			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Map]")==NULL)
			{
				probeMap(buffer.c_str(), *load_location_map);
			}
		}
	}


	file_to_load.close();

	return 0;
}



