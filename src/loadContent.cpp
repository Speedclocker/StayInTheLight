#include "loadContent.h"
#include "ResourcesManagement.h"
#include "Character.h"
#include "Collector.h"

#define ENTITIES_FOLDER "data/entities"
#define LINE_SIZE 1000


/*-------------------------------------------- Load Map -----------------------------------------------------*/


void probeEntity(const char* raw_line, std::vector<Entity*>* entities, Map* load_location_map, ResourcesManager* resources_manager)
{
	char line[BUFF_SIZE];
	strcpy(line, raw_line);

	char* buffer=NULL;
	std::string id, entity_model_name;
	int height = 0;
	sf::Vector2f position = sf::Vector2f(0,0);

	
	buffer = strtok(line, " :\n");
	if(buffer != NULL) id = buffer;


	buffer = strtok(NULL, " (,:\n");
	if(buffer != NULL) entity_model_name = buffer;


	buffer = strtok(NULL, " (,:\n");
	if(buffer != NULL) height = atoi(buffer);
		

	buffer = strtok(NULL, " (,:\n");
	if(buffer != NULL) position.x = atof(buffer);


	buffer = strtok(NULL, " )(,:\n");
	if(buffer != NULL) position.y = atof(buffer);


	Entity* entity_to_load = NULL;


	// Set an id and check if it is already in vector
	int index = 0;
	bool name_not_taken = true;
	std::string name; 
	std::string index_str;
	
	do
	{
		name_not_taken = true;
		index_str = ((index < 10)?"0":"") + std::to_string(index);
		if(index == 0)
			name = id;
		else
			name = id + "_" + index_str;

		for(std::vector<Entity*>::iterator it = entities->begin(); it != entities->end() && name_not_taken; it++)
		{
			if(strcmp((*it)->getID().c_str(),name.c_str())==0)
				name_not_taken = false;
		}

		index++;

	} while(!name_not_taken);
	//////////////

	std::string entity_file_name =  ENTITIES_FOLDER + std::string("/") + entity_model_name + std::string(".ent");

	if(loadEntity(name, entity_file_name, resources_manager, &entity_to_load, load_location_map) >= 0)
	{
		entity_to_load->setPosition(sf::Vector2f(position.x, position.y));
		entity_to_load->setHeight(height);
		entities->push_back(entity_to_load);
		load_location_map->addEntity(entity_to_load);
	}

}



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


int loadMap(Map** load_location_map, std::vector<Entity*>* entities_to_load, std::string map_to_load_file_name, ResourcesManager* resources_manager)
{
	// Load the map 

	std::ifstream file_to_load;
	file_to_load.open(map_to_load_file_name);
	if(file_to_load.fail()) {std::cerr << "Une erreur est survenue lors de la tentative d'ouverture du fichier " << map_to_load_file_name << std::endl; file_to_load.close(); return -1; }

	sf::Vector2f size_map=sf::Vector2f(-1,-1);
	int height_map=-1, tile_sz_map=-1;
	sf::Texture* texture;
	bool texture_load = false;
	char texture_name_file[BUFF_SIZE];
	std::string texture_name_file_string;

	std::string buffer="";

	while(std::getline(file_to_load, buffer))
	{
		// Read the file until we reach the end
		if(strstr(buffer.c_str(), "[Header]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Header]")==NULL)
			{
				probeHeader(buffer.c_str(), &size_map, &height_map, &tile_sz_map, texture_name_file);
			}

			texture_name_file_string = texture_name_file;

			//En cas d'erreur
			if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }

			if((texture=resources_manager->getOrAddTexture(texture_name_file_string)) == NULL){ std::cerr << "Une erreur lors du chargement de texture" << std::endl; file_to_load.close(); return -1; }
			else texture_load = true;


			// If no error during map loading, we free the map
			if(*load_location_map != NULL)
			{
				delete *load_location_map;
				*load_location_map=NULL;
			}


			// Clear entities
			for(std::vector<Entity*>::iterator it = entities_to_load->begin(); it != entities_to_load->end(); it++)
				delete (*it);
			entities_to_load->clear();


			// Creation of the map
			*load_location_map = new Map(size_map, height_map, tile_sz_map);

			(*load_location_map)->setTexture(texture);
		}


		else if(strstr(buffer.c_str(), "[Map]")!=NULL)
		{
			if(size_map==sf::Vector2f(-1,-1) || height_map==-1 || tile_sz_map==-1){ std::cerr << "Une erreur a eu lieu lors de la lecture d'un des paramètres" << std::endl; file_to_load.close(); return -1; }

			if(!texture_load && (texture=resources_manager->getOrAddTexture(texture_name_file_string)) == NULL){ std::cerr << "Une erreur lors du chargement de texture" << std::endl; file_to_load.close(); return -1; }
			else texture_load = true;

			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Map]")==NULL)
			{
				probeMap(buffer.c_str(), *load_location_map);
			}
		}

		else if(strstr(buffer.c_str(), "[Entities]")!=NULL)
		{
			while(std::getline(file_to_load, buffer) && strstr(buffer.c_str(), "[/Entities]")==NULL)
			{
				probeEntity(buffer.c_str(), entities_to_load, *load_location_map, resources_manager);
			}
		}

	}


	file_to_load.close();

	return 0;
}




// ------------------------------------ Load Entity -----------------------------------




int loadEntity(std::string entity_id, std::string file_name, ResourcesManager* resources_manager, Entity** entity_to_load, Map* entity_location)
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

	if(strcmp(type.c_str(), "")==0) {std::cerr << "An error happened while loading entity : type is unknown" << std::endl; return -1; }




	// --------- Entity loading ----------

	try
	{
		if(strcmp(type.c_str(), "Character")==0)			*entity_to_load = new Character(entity_id, file_name, resources_manager, entity_location);
		else if(strcmp(type.c_str(), "Collector")==0)		*entity_to_load = new Collector(entity_id, file_name, resources_manager, entity_location);
	}
	catch(const std::string & e)
	{
		// If an error occured while loading the entity
		if(*entity_to_load != NULL)
		{
			delete *entity_to_load;
			*entity_to_load = NULL;
		}
		return -1;
	}
	


	return 0;
}

