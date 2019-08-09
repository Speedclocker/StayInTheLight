#ifndef __RESOURCES_MANAGEMENT_H__
#define __RESOURCES_MANAGEMENT_H__

#include <SFML/Graphics.hpp>
#include <iostream>

class ResourcesManager
{
public:
	ResourcesManager();
	~ResourcesManager();

	
	sf::Texture* 		getTexture(std::string texture_name);
	int 				newTexture(std::string texture_file_name);
	sf::Texture* 		getOrAddTexture(std::string texture_name);
	int 				delTexture(std::string texture_name);



private:
	std::map<std::string, sf::Texture*> m_textures;
};










































#endif