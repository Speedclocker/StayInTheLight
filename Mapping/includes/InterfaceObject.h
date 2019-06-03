#ifndef __INTERFACEOBJECT_H__
#define __INTERFACEOBJECT_H__


#include <SFML/Graphics.hpp>
#include <iostream>

#include "Map.h"

class InterfaceObject : public sf::Drawable
{
public:
	std::string			getID();
	sf::Vector2f 		getSize();
	sf::Vector2f 		getPosition();

	void				setID(std::string id);
	void 				setSize(sf::Vector2f size);
	void 				setPosition(sf::Vector2f position);

	virtual void		update();
protected:
	std::string			m_id;
	sf::Vector2f		m_position;
	sf::Vector2f		m_size;
};




class TilesetSelect : public InterfaceObject
{
public: 
	//Constructeurs/Destructeurs
	TilesetSelect();
	~TilesetSelect();
	TilesetSelect(const std::string id, sf::Texture* texture, sf::Rect<float> zone, Tile tiles[], int nbr_available_tiles, int size_tile);

	//Accesseurs
	sf::Vector2f		getPosition();
	sf::Vector2f		getSize();
	sf::Rect<float>		getZone();
	sf::Vector2f		getMaxZonePos();
	Tile 				getChosenTile();

	//Modificateurs
	void 				setPosition(sf::Vector2f position);
	void				setZone(sf::Rect<float> zone);
	void				setSize(sf::Vector2f size);

	//Méthodes
	void				collision_settings(sf::RenderWindow* window);
	void 				choice_tile(sf::RenderWindow* window);
	void 				interactsWithUser(sf::RenderWindow* window);
	void 				update();
	void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Rect<float>		m_zone;
	sf::VertexArray		m_vertex;
	sf::Texture*		m_texture;
	sf::Texture 		m_state_text;


	Tile* 				m_tiles;
	int 				m_nbr_tiles;
	int 				m_chosen_tile;
	sf::Vector2f		m_chosen_tile_pos_text;
	int 				m_size_tile;

	enum State {NONE, HOVER, CLICK};
	State 				m_state;
};



class SlideBar : public InterfaceObject
{
public: 
	enum Type { HORIZONTAL, VERTICAL };

	//Constructeurs/Destructeurs
	SlideBar();
	~SlideBar();
	SlideBar(std::string id, float valMin, float valMax);


	//Accesseurs
	float			getMinValue();
	float			getMaxValue();
	float			getCurrentValue();	
	Type 			getType();


	//Modificateurs
	void			setMinValue(float minValue);
	void			setMaxValue(float maxValue);
	void			setCurrentValue(float currentValue);
	void 			setCurrentValueFromPos(sf::Vector2f clickPos);

	void 			setType(Type type);


	//Méthodes
	void 			interactsWithUser(sf::RenderWindow* window);
	void 			update();
	void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	float				m_minValue, m_maxValue, m_currentValue;
	Type				m_type;

	enum State {NONE, HOVER, CLICK};
	State 				m_state;
};



#endif