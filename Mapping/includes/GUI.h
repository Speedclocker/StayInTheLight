#ifndef __GUI_H__
#define __GUI_H__

#include <iostream>
#include "BoxWindow.h"
#include "functionTabs.h"	


class Map;


class MappingGUI
{
public:
	MappingGUI();
	~MappingGUI();
	MappingGUI(sf::RenderWindow* window);
	MappingGUI(sf::RenderWindow* window, Map* map, std::vector<Entity*>* entities, ResourcesManager* resources_manager, std::string tileset_texture_file_name);


	//Getters
	sf::RenderWindow*							getWindow(void);
	BWManager* 									getBWManager(void);
	std::string* 								getStringPtrToTextEvent(void);

	//Setters
	void 										setWindow(sf::RenderWindow* window);
	int 										setTilesetTexture(sf::Texture* tileset_texture);
	int											setMap(Map* map);


	//Methods
	void										setTile();
	void 										setEntity();
	void 										initializeBoxWindows();
	void 										controlView();
	void 										heightLevelCommand();
	void 										interactWithUser();
	void 										update();
	void 										draw();


private:
	sf::RenderWindow*							m_window;
	BWManager* 									m_windows_manager;
	ResourcesManager* 							m_resources_manager;
	sf::Font 									m_font;
	Map* 										m_map;
	std::vector<Entity*>* 						m_entities;
	int 										m_tile_size;
	sf::Texture*								m_tileset_texture;
	std::string 								m_tileset_texture_file_name;
	Tile* 										m_available_tiles;
	int 										m_nbr_available_tiles;
	std::string* 								m_ptr_event_txt_entered;
	int 										m_chosen_height;
	Tile 										m_chosen_tile;
	Entity* 									m_chosen_entity;
	std::string 								m_chosen_entity_file_name;
	bool 										m_transparency_map;

	// Used for setting tile
	bool 										multiple_setting_tile_click;
	sf::Vector2f 								set_tile_origin_click, set_tile_corner_click;

	// Used for setting entity
	std::vector<std::string>					entities_file_name_list;
	bool										set_entity_click;

	// Used for moving view
	bool 										move_view_click;
	sf::Vector2f	 							move_view_origin_click;

	// Used for changing current height
	bool 										keyPressed_lower_layer, keyPressed_higher_layer, keyPressed_transparency;
};

















































#endif