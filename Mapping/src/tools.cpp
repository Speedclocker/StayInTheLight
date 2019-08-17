#include "tools.h"


#define ENTITIES_FOLDER "entities"
#define CHARACTER_SIZE_HEIGHT 16

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
	std::cout << "Pour sauvegarder la map, maintenez LCtrl et appuyez S. Les maps sont enregistrées dans le dossier ./maps" << std::endl;
	std::cout << "Pour charger une map, maintenez LCtrl et appuyez L" << std::endl;
}

