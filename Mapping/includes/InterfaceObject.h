#ifndef __INTERFACEOBJECT_H__
#define __INTERFACEOBJECT_H__


#include <SFML/Graphics.hpp>
#include <iostream>

#include "Map.h"


class AnimatedSprite;



// Le pointeur qui redigera les entrées claviers
extern std::string* PTR_EVENT_TEXT_ENTERED;



/////////////////////// InterfaceObject ////////////////////////
/* InterfaceObject is a mother-class containing all interface object classes such as buttons, slidebars... useful for user interactions*/

class InterfaceObject : public sf::Drawable
{
public:
	//Constructors/Destructors
	InterfaceObject();
	~InterfaceObject();


	//Getters
	std::string			getID();
	sf::Vector2f 		getSize();
	sf::Vector2f 		getPosition();


	//Setters
	void				setID(std::string id);
	void 				setSize(sf::Vector2f size);
	void 				setPosition(sf::Vector2f position);

	virtual void		update();
	virtual void 		interactsWithUser(sf::RenderWindow* window);
protected:
	std::string			m_id;
	sf::Vector2f		m_position;
	sf::Vector2f		m_size;
};





////////////////////////// InputBar /////////////////////////////
/* Child class allowing user to type text inside a box */

class InputBar : public InterfaceObject
{
public:
	enum Type 		{NUMERICAL, ALPHANUMERICAL};
	enum State 		{NONE, HOVER, CLICK};

	//Constructors/Destructors
	InputBar();
	~InputBar();
	InputBar(std::string id, int size_font, int size_long, InputBar::Type type, std::string** ptr_event_txt_entered_associated);


	//Getters
	sf::Vector2f	getPosition();
	sf::Vector2f	getSize();
	State 			getState();
	std::string		getValue();
	Type 			getType();

	//Setters
	void			setPosition(sf::Vector2f position);
	void 			setSize(sf::Vector2f size);
	void 			setType(Type type);
	void  			setValue(std::string buff);


	//Methods
	bool 			hasFocus();
	void			interactsWithUser(sf::RenderWindow* window);
	void 			update();
	void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	std::string** 	m_ptr_event_txt_entered_associated;
	int 			m_size_font;

	std::string*	m_buff;
	sf::Font		m_font;

	clock_t 		m_time;
	bool 			m_typing_cursor;

	State			m_state;
	Type 			m_type;

	bool			m_keyboard_pressed;
	bool			m_focus;

};







/////////////////////////// Button //////////////////////////////
/* Child class which is a simple button */


class Button : public InterfaceObject
{
public:
	enum State {NONE, HOVER, CLICK};

	//Constructors/Destructors
	Button();
	~Button();
	Button(std::string id, std::string text, int size_font);


	//Getters
	sf::Vector2f	getPosition();
	sf::Vector2f	getSize();
	State 			getState();


	//Setters
	void			setPosition(sf::Vector2f position);
	void 			setSize(sf::Vector2f size);


	//Methods
	bool 			isClick();
	void			interactsWithUser(sf::RenderWindow* window);
	void 			update();
	void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	int 			m_size_font;

	std::string		m_text;
	sf::Font		m_font;


	State			m_state;
	bool			m_isClick;

};




/////////////////////////// TilesetSelect /////////////////////////////////
/* Child class allowing user to select tile in a tileset, to manage collisions for each tile etc... */


class TilesetSelect : public InterfaceObject
{
public: 
	//Constructors/Destructors
	TilesetSelect();
	~TilesetSelect();
	TilesetSelect(const std::string id, const sf::Texture* texture, sf::Rect<float> zone, Tile** tiles, const int* nbr_available_tiles, const int* size_tile, Tile* target_tile);

	//Getters
	sf::Vector2f		getPosition();
	sf::Vector2f		getSize();
	sf::Rect<float>		getZone();
	sf::Vector2f		getMaxZonePos();
	Tile 				getChosenTile();
	int 				getTileSize();

	//Setters
	void 				setPosition(sf::Vector2f position);
	void				setZone(sf::Rect<float> zone);
	void				setSize(sf::Vector2f size);
	void				setSizeTile(int* size_tile);

	//Methods
	void				collision_settings(sf::RenderWindow* window);
	void 				choice_tile(sf::RenderWindow* window);
	void 				interactsWithUser(sf::RenderWindow* window);
	void 				update();
	void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Rect<float>		m_zone;
	sf::VertexArray		m_vertex;
	sf::Texture 		m_state_texture;

	int 				m_chosen_tile;
	sf::Vector2f		m_chosen_tile_pos_text;


	// Pointeurs
	const sf::Texture*		m_texture;
	Tile** 					m_ptr_tiles;
	const int* 				m_nbr_tiles;
	const int*	 			m_size_tile;
	Tile*			 		m_target_tile;



	enum State {NONE, HOVER, CLICK};
	State 				m_state;
};





/////////////////////////// SlideBar /////////////////////////////////
/* Child class building a sliding bar */


class SlideBar : public InterfaceObject
{
public: 
	enum Type { HORIZONTAL, VERTICAL };

	//Constructors/Destructors
	SlideBar();
	~SlideBar();
	SlideBar(std::string id, float valMin, float valMax);


	//Getters
	float			getMinValue() const;
	float			getMaxValue() const;
	float			getCurrentValue() const;	
	Type 			getType() const;


	//Setters
	void			setMinValue(float minValue);
	void			setMaxValue(float maxValue);
	void			setCurrentValue(float currentValue);
	void 			setCurrentValueFromPos(sf::Vector2f clickPos);
	void 			setRatioCursorBar(float ratio_cursor_bar);
	void 			setType(Type type);


	//Methods
	void 			interactsWithUser(sf::RenderWindow* window);
	void 			update();
	void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	float				m_minValue, m_maxValue, m_currentValue;
	float				m_ratio_cursor_bar;
	Type				m_type;

	enum State {NONE, HOVER, CLICK};
	State 				m_state;
};





/////////////////////////// Scrolling List /////////////////////////////////
/* Child class building a scrolling list */


class ScrollingList : public InterfaceObject
{
public: 
	//Constructors/Destructors
	ScrollingList();
	~ScrollingList();
	ScrollingList(std::string id, int font_height, int unrollList_height, int width);



	//Getters
	int 			getWidth();
	int 			getFontHeight();
	std::string		getCurrentValue();	
	int 			getNbrValues();
	bool 			isUnroll();


	//Setters
	void 			setWidth(int width);
	void 			setFontHeight(int font_height);
	void			setCurrentValue(std::string currentValue);


	//Methods
	void			addValue(std::string value);
	void 			clearList();
	void 			interactsWithUser(sf::RenderWindow* window);
	void 			update();
	void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	enum State {NONE, HOVER, CLICK};
	struct Option {std::string value; ScrollingList::State state;};

	State 						m_state;
	int 						m_width;
	int 						m_unrollList_height;
	int 						m_font_height;
	int 						m_nbrValues;
	std::vector<struct Option>	m_values;
	std::string					m_currentValue;
	sf::Font 					m_font;
	SlideBar 					m_scrollingBar;
	bool 						m_unroll;
	sf::RenderTexture			m_texture;

};




/////////////////////////// TextZone /////////////////////////////////
/* Child class building a text zone */


class TextZone : public InterfaceObject
{
public: 
	//Constructors/Destructors
	TextZone();
	~TextZone();
	TextZone(std::string id, int font_size, std::string text, bool background);


	//Getters
	int 			getFontSize();
	std::string 	getText();
	bool 			hasBackground();


	//Setters
	void 			setFontSize(int font_size);
	void 			setText(std::string text);
	void 			setBackground(bool background);


	//Methods
	void 			update();
	void 			draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	int 							m_font_size;
	std::string 					m_text_string;
	bool 							m_background;
	sf::Text 						m_text;
	sf::Font 						m_font;
};









/////////////////////////// EntityDisplayer /////////////////////////////////
/* Child class building a zone displaying entity */


class EntityDisplayer : public InterfaceObject
{
public: 
	//Constructors/Destructors
	EntityDisplayer();
	~EntityDisplayer();
	EntityDisplayer(std::string id, bool background);


	//Getters
	bool 				hasBackground();
	Entity* 			getEntity();


	//Setters
	void 				setBackground(bool background);
	void 				setEntity(Entity* entity);


	//Methods
	void 				update();
	void 				draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	bool 							m_background;
	Entity* 						m_entity;
	AnimatedSprite* 				m_sprite;

};








#endif










