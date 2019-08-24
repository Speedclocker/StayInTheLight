#include <cstring>

#include "ResourcesManagement.h"

ResourcesManager::ResourcesManager()
{

}


ResourcesManager::~ResourcesManager()
{
	for(std::map<std::string, sf::Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		delete it->second;
	}
	m_textures.clear();
}


sf::Texture* ResourcesManager::getTexture(std::string texture_name)
{
	std::map<std::string, sf::Texture*>::iterator found_texture = m_textures.find(texture_name);

	if(found_texture != m_textures.end())
		return found_texture->second;

	for(std::map<std::string, sf::Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if(strstr(("/"+it->first+"\0").c_str(), texture_name.c_str())!=NULL)
			return it->second;
	}

	return NULL;
}


int ResourcesManager::newTexture(std::string texture_file_name)
{
	sf::Texture* tmp_texture = new sf::Texture();
	if(!tmp_texture->loadFromFile(texture_file_name)) 
	{
		std::cerr << "An error occured while loading texture (ResourcesManager::newTexture) : texture has not been found." << std::endl; 
		delete tmp_texture; 
		return -1;
	}

	m_textures.insert(std::pair<std::string, sf::Texture*>(texture_file_name, tmp_texture));

	return 1;

}


sf::Texture* ResourcesManager::getOrAddTexture(std::string texture_name)
{
	sf::Texture* texture;
	if( (texture = this->getTexture(texture_name)) == NULL)
	{ 
		if(this->newTexture(texture_name) < 0) { std::cerr << "Error while loading of texture " << texture_name << std::endl; return NULL; }
		else if((texture = this->getTexture(texture_name)) == NULL) { std::cerr << "Error while loading of texture " << texture_name << std::endl; return NULL; }
	}

	return texture;
}


int ResourcesManager::delTexture(std::string texture_name)
{
	std::map<std::string, sf::Texture*>::iterator found_texture = m_textures.find(texture_name);

	if(found_texture != m_textures.end())
	{
		m_textures.erase(found_texture);
		return 1;
	}
	
	return 0;
}
