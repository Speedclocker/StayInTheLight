#ifndef __BOXWINDOW_H__
#define __BOXWINDOW_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#define FONT_FILE "AldoTheApache.ttf"


/* Classes permettant de construire les fenêtres applications, avec les différents onglets contenant le tileset, les entités à ajouter, les paramètres etc ... */

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
	TilesetSelect(std::string id, sf::Texture* texture, sf::Rect<float> zone);

	//Accesseurs
	sf::Rect<float>		getZone();
	sf::Vector2f		getMaxZonePos();

	//Modificateurs
	void 				setPosition(sf::Vector2f position);
	void				setSize(sf::Vector2f size);
	void				setZone(sf::Rect<float> zone);

	//Méthodes
	void 				interactsWithUser(sf::RenderWindow* window);
	void 				update();
	void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Rect<float>		m_zone;
	sf::VertexArray		m_vertex;
	sf::Texture*		m_texture;
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





class Tab : public sf::Drawable
{
public:
	//Constructeurs/Destructeurs
	Tab();
	Tab(std::string name_title, sf::RenderWindow *window, void (*function)(void*));
	~Tab();


	//Accesseurs
	sf::Vector2f 					getSize();
	float							getTitleSize();
	sf::Vector2f 					getPosition();
	float 							getXOffset();
	std::string						getTitle();
	sf::RenderWindow*				getRenderWindow();
	InterfaceObject*				getObject(std::string objectID);

	//Modificateurs
	void 							setSize(sf::Vector2f size);
	void							setTitleSize(float title_size);
	void 							setPosition(sf::Vector2f position);
	void 							setXOffset(float offset);
	void							setTitle(std::string title);

	//Méthodes
	void							Function(void* arg);
	void							addObject(InterfaceObject* object);
	void							update();
	void 							draw(sf::RenderTarget& target, sf::RenderStates states) const;


	//Public variable
	bool 											initialized;

private:
	void 											(*tabFunction)(void*);

	std::map< std::string, InterfaceObject*>		m_objects;
	sf::Vector2f 									m_size;
	float											m_title_size;
	sf::Vector2f 									m_position;
	float											m_x_offset_title_pos;

	std::string				 						m_title;
	sf::Font										m_font;
	sf::RenderWindow								*m_window;

};





class BoxWindow : public sf::Drawable
{
public:
	//Constructeurs/Destructeurs
	BoxWindow();
	BoxWindow(sf::Vector2f size, sf::RenderWindow* window);
	~BoxWindow();


	//Accesseurs
	Tab*				getTab(int index);
	sf::Vector2f 		getSize();
	sf::Vector2f 		getPosition();
	sf::Vector2f		getMinSize();
	sf::Vector2f		getMaxSize();
	float				getTabTitleSize();


	//Modificateurs
	void 				setSize(sf::Vector2f size);
	void	 			setPosition(sf::Vector2f position); 
	void				setPositionWindow(sf::Vector2f position);
	void				setMinSize(sf::Vector2f minSize);
	void				setMaxSize(sf::Vector2f maxSize);
	void				setTabTitleSize(float tabtitle_size);

	//Méthodes
	void 				addTab(Tab* tab);
	void				move_resize();
	void				interactsWithUser();

	void 				update();
	void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Tab*>	m_tabs;
	sf::Vector2f		m_position;	
	sf::Vector2f		m_position_window; // Il s'agit de la position relative à la View de la RenderWindow prise en paramètre et non des coordonnées absolues
	sf::Vector2f		m_size;
	float				m_tabtitle_size;
	sf::Vector2f		m_minSize, m_maxSize;

	enum State {NONE, LEFT, RIGHT, BOTTOM, TOP};
	State 				m_state_hover;
	State 				m_state_click;
	sf::RenderWindow	*m_window;
};



#endif