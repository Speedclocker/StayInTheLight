#include "saveMap.h"
#include "Character.h"
#include "Collector.h"

#define LINE_SIZE 1000

void probeTileset(const char* raw_line, Tile* tileset, int tileset_size)
{
	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;

	if(strstr(line, "|")!=NULL)
	{	
		buffer=strtok(line, "|/ ");

		int i=0;
		while(buffer!=NULL && i<tileset_size)
		{
			if(atoi(buffer)==0) tileset[i].m_collisionable = false;
			else tileset[i].m_collisionable = true;

			buffer=strtok(NULL, "|/ ");
			i++;
		}
	}
}



void probeMap(const char* raw_line, Map* load_location_map)
{
	static int actual_height=0, actual_X=0;
	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;

	if(strstr(line, "[H")!=NULL)
	{
		//Detection d'un changement de hauteur
		buffer = strtok(line, "H\n");
		buffer = strtok(NULL, "]\n");

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


int loadMap(Map** load_location_map, std::string name_file_map_to_load, std::string* name_texture_file, sf::Texture* texture, Tile** tileset, int* tileset_size)
{
	std::ifstream file_to_load;
	file_to_load.open("maps/"+name_file_map_to_load);
	if(file_to_load.fail()) {std::cerr << "Une erreur est survenue lors de la tentative d'ouverture du fichier " << name_file_map_to_load << std::endl; file_to_load.close(); return -1; }

	sf::Vector2f size_map=sf::Vector2f(-1,-1);
	int height_map=-1, tile_sz_map=-1;
	bool texture_load = false;
	char texture_name_file[BUFF_SIZE];

	std::string buffer="";


	while(std::getline(file_to_load, buffer))
	{
		if(strstr(buffer.c_str(), "[Header]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Header]")==NULL)
			{
				probeHeader(buffer.c_str(), &size_map, &height_map, &tile_sz_map, texture_name_file);
			}

			*name_texture_file = texture_name_file;

			//En cas d'erreur
			if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }
			if(!texture->loadFromFile(*name_texture_file)){ std::cerr << "Une erreur lors du chargement de texture" << std::endl; file_to_load.close(); return -1; }
			else texture_load = true;

			// If no error during map loading, we free the map
			if(*load_location_map != NULL)
			{
				delete *load_location_map;
				*load_location_map=NULL;
			}


			// Creation of the map
			*load_location_map = new Map(size_map, height_map, tile_sz_map);
			

			//Modification des tiles dispo:
			*tileset_size=(texture->getSize().x/tile_sz_map)*(texture->getSize().y/tile_sz_map);
			*tileset=(Tile*)realloc((*tileset),(*tileset_size)*sizeof(Tile));


			for(int i=0; i<*tileset_size;i++)
			{
				(*tileset)[i].m_pos_text = sf::Vector2f( (i%(texture->getSize().x/tile_sz_map))*tile_sz_map,  (i/(texture->getSize().x/tile_sz_map))*tile_sz_map );
				(*tileset)[i].m_size_text = sf::Vector2f( tile_sz_map, tile_sz_map );
				(*tileset)[i].m_collisionable = false;
			}
		}


		else if(strstr(buffer.c_str(), "[Tileset]")!=NULL)
		{
			//std::cout << "tileset" << std::endl;
			
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Tileset]")==NULL)
			{
				probeTileset(buffer.c_str(), *tileset, *tileset_size);
			}
		}


		else if(strstr(buffer.c_str(), "[Map]")!=NULL)
		{
			//std::cout << "map" << std::endl;
			if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }

			if(!texture_load && !texture->loadFromFile(*name_texture_file)){ std::cerr << "Une erreur lors du chargement de texture" << std::endl; file_to_load.close(); return -1; }
			else texture_load = true;

			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Map]")==NULL)
			{
				probeMap(buffer.c_str(), *load_location_map);
			}
		}
	}
	file_to_load.close();

	std::cout << "end" <<std::endl;

	return 0;
}



int saveMap(Map* map_to_save, std::string name_map, std::string name_file, std::string texture_name_file, Tile* tileset, int tileset_size)
{
	std::ofstream file_to_save;

	file_to_save.open("maps/"+name_file);
	if(file_to_save.fail()) {std::cerr << "Une erreur est survenue lors de la tentative de création du fichier " << name_file << std::endl; file_to_save.close(); return -1; }

	//Header
	file_to_save << "[Header]" << std::endl;

	////Enregistrement du nom 
	file_to_save << "Name : " << name_map <<std::endl;

	////Enregistrement de la taille d'une size
	file_to_save << "Tile Size : " << map_to_save->getTileSize() << std::endl;

	////Enregistrement de la Hauteur de la map
	file_to_save << "Height : " << map_to_save->getHeight() << std::endl;

	////Enregistrement de la taille de la map
	file_to_save << "X : " << map_to_save->getSize().x << std::endl;
	file_to_save << "Y : " << map_to_save->getSize().y << std::endl;

	////Enregistrement du nom du fichier de texture
	file_to_save << "Texture File : " << texture_name_file << std::endl;

	//End of Header
	file_to_save << "[/Header]" << std::endl;


	file_to_save << std::endl;


	//Tileset
	file_to_save << "[Tileset]" << std::endl;

	////Collisionable or not
	file_to_save << "| " ;
	for(int i=0; i < tileset_size; i++)
	{
		file_to_save << tileset[i].m_collisionable ;
		if(i<tileset_size-1)
			file_to_save << "/" ;
	}
	file_to_save << " |" << std::endl;

	//End Tileset
	file_to_save << "[/Tileset]" << std::endl;


	file_to_save << std::endl;


	//Map
	file_to_save << "[Map]" << std::endl;
	for(int h=0; h<map_to_save->getHeight(); h++)
	{
		file_to_save << "[H" << h << "]" << std::endl;
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
				
				file_to_save << "(" << tmp_tile.m_pos_text.x/map_to_save->getTileSize() << "," << tmp_tile.m_pos_text.y/map_to_save->getTileSize() << "-" << tmp_tile.m_collisionable << ") " ;
				
			}
			file_to_save << "|" << std::endl ;
		}
		file_to_save << "[/H" << h << "]" << std::endl;
	}
	file_to_save << "[/Map]" << std::endl;

	file_to_save.close();

	return 0;
}





int loadEntity(std::string file_name, sf::Texture* texture, Entity** entity_to_load, Map* entity_location)
{
	if(*entity_to_load!=NULL) {std::cerr << "An error happened while loading entity : entity in parameters has to be NULL" << std::endl; return -1; }

	std::ifstream file_to_load;
	file_to_load.open(file_name);
	if(file_to_load.fail()) {std::cerr << "An error happened while opening the entity file" << file_name << std::endl; file_to_load.close(); return -1; }

	std::string type="";

	std::string buffer="";


	// Read the file until we reach the end
	while(std::getline(file_to_load, buffer))
	{
		// Header Tag
		if(strstr(buffer.c_str(), "[Header]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Header]")==NULL)
			{
				char line[LINE_SIZE];
				strcpy(line, buffer.c_str());

				strtok(line, ": \n");
				char* tag = strtok(NULL, ": \n");

				
				if(strstr(buffer.c_str(), "Type : ")!=NULL && tag!=NULL)
					type = tag;
				

			}
		}
	}

	file_to_load.close();

	std::cout << "SPOT1.1 : " << type<< std::endl;

	if(strcmp(type.c_str(), "")==0) {std::cerr << "An error happened while loading entity : type is unknown" << std::endl; return -1; }




	// --------- Entity loading ----------

	try
	{
		if(strcmp(type.c_str(), "Character")==0)
		{
			*entity_to_load = new Character(file_name, texture, entity_location);
		}	
		else if(strcmp(type.c_str(), "Collector")==0)
		{
			*entity_to_load = new Collector(file_name, texture, entity_location);
		}
	}
	catch(const std::string & e)
	{
		// If an error occured while loading the entity
		if(*entity_to_load != NULL)
		{
			delete *entity_to_load;
			*entity_to_load = NULL;
		}
	}
	


	return 0;
}