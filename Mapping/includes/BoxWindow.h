#define ARG_TAB_BUFF_SIZE 100

#ifndef __BOXWINDOW_H__
#define __BOXWINDOW_H__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "InterfaceObject.h"




typedef struct{uint8_t val[ARG_TAB_BUFF_SIZE];} ArgTab;

/////////////////////////////////////////////////////////////////////////////// Tab /////////////////////////////////////////////////////////////////////////////////////////

/* Classes permettant de construire les fenêtres applications, avec les différents onglets contenant le tileset, les entités à ajouter, les paramètres etc ... */

class Tab : public sf::Drawable
{
public:
	//Constructeurs/Destructeurs
	Tab();
	Tab(std::string name_title, sf::RenderWindow *window, void (*function)(Tab*, ArgTab*), ArgTab* function_arg, size_t size_function_arg);
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
	void 							interactsWithUser(sf::RenderWindow* window);
	void							Function();
	void							addObject(InterfaceObject* object);
	void							update();
	void 							draw(sf::RenderTarget& target, sf::RenderStates states) const;


	//Public variable
	bool 											initialized;

private:
	void 											(*tabFunction)(Tab*, ArgTab*);
	ArgTab*											m_function_arg;

	InterfaceObject*								m_focus_object;
	std::map< std::string, InterfaceObject*>		m_objects;
	sf::Vector2f 									m_size;
	float											m_title_size;
	sf::Vector2f 									m_position;
	float											m_x_offset_title_pos;


	std::string				 						m_title;
	sf::Font										m_font;
	sf::RenderWindow								*m_window;

};



/////////////////////////////////////////////////////////////////////////////// BoxWindow /////////////////////////////////////////////////////////////////////////////////////////


class BoxWindow : public sf::Drawable
{
public:
	//Constructeurs/Destructeurs
	BoxWindow();
	BoxWindow(sf::Vector2f size, sf::RenderWindow* window);
	~BoxWindow();


	//Accesseurs
	Tab*				getTab(int index);
	Tab* 				getFocusTab();
	sf::Vector2f 		getSize();
	sf::Vector2f 		getPosition();
	sf::Vector2f		getMinSize();
	sf::Vector2f		getMaxSize();
	float				getTabTitleSize();
	bool				isInteractable();


	//Modificateurs
	void 				setSize(sf::Vector2f size);
	void	 			setPosition(sf::Vector2f position); 
	void				setPositionWindow(sf::Vector2f position);
	void				setMinSize(sf::Vector2f minSize);
	void				setMaxSize(sf::Vector2f maxSize);
	void				setTabTitleSize(float tabtitle_size);

	//Méthodes
	void 				setInteractable();
	void 				unInteractable();
	void  				focusTab(Tab* tab);
	void 				addTab(Tab* tab);
	void 				newTab(std::string tab_name, void (*function)(Tab*, ArgTab*), ArgTab* function_arg, size_t function_arg_size);
	void				move_resize();
	void				interactsWithUser();

	void 				Function();
	void 				update();
	void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	//std::map<std::string, Tab*>		m_tabs1;
	std::vector<Tab*>				m_tabs;
	Tab* 							m_focus_tab;
	sf::Vector2f					m_position;	
	sf::Vector2f					m_position_window; // Il s'agit de la position relative à la View de la RenderWindow prise en paramètre et non des coordonnées absolues
	sf::Vector2f					m_size;
	sf::Vector2f					m_minSize, m_maxSize;
	float							m_tabtitle_size;
	bool 							m_interactable;
	sf::Font 						m_font;

	enum State {NONE, LEFT, RIGHT, BOTTOM, TOP};
	State 							m_state_hover;
	State 							m_state_click;
	sf::Vector2f 					m_click_move_window;
	sf::RenderWindow*				m_window;
};






/////////////////////////////////////////////////////////////////////////////// BoxWindowsManager /////////////////////////////////////////////////////////////////////////////////////////

class BWManager
{
public:
	BWManager();
	~BWManager();
	BWManager(sf::RenderWindow* window);


	//Getters
	sf::RenderWindow*							getWindow(void);
	BoxWindow* 									getBoxWindow(std::string window_name);
	BoxWindow* 									getFocusBoxWindow();


	//Setters
	void 										setWindow(sf::RenderWindow* window);


	//Methods
	void 										newBoxWindow(std::string boxwindow_name, sf::Vector2f size);
	void 										deleteBoxWindow(std::string boxwindow_name);
	void 										clearBoxWindows();
	void 										interactionsManagement();
	void 										update();
	void 										blockOnFocusWindow();
	void 										unblockOnFocusWindow();
	void 										focusOnBoxWindow(BoxWindow* box_window);
	void 										unFocusBoxWindow();
	void 										drawWindows();
	void 										manage();


private:
	sf::RenderWindow*							m_window;
	std::map<std::string, BoxWindow*> 			m_box_windows;
	std::vector<BoxWindow*>						m_order_box_windows;

	const BoxWindow* 							m_window_hover;
	const BoxWindow* 							m_window_click;
	BoxWindow* 									m_focus_window;
	bool 										m_block_on_focus_window;

};



#endif