#include "BoxWindow.h"

int BOXWINDOW_SIZE_CHARACTER = 15 ;



std::string InterfaceObject::getID()
{
	return m_id;
}

sf::Vector2f InterfaceObject::getPosition()
{
	return m_position;
}

sf::Vector2f InterfaceObject::getSize()
{
	return m_size;
}




void InterfaceObject::setID(std::string id)
{
	m_id = id;
}

void InterfaceObject::setSize(sf::Vector2f size)
{
	m_size=size;
}

void InterfaceObject::setPosition(sf::Vector2f position)
{
	m_position = position;
}


void InterfaceObject::update()
{

}



//////////////////////////////////////////////////////////// TilesetSelect //////////////////////////////////////////////////////////



TilesetSelect::TilesetSelect()
{

}


TilesetSelect::~TilesetSelect()
{

}


TilesetSelect::TilesetSelect(std::string id, sf::Texture* texture, sf::Rect<float> zone)
{
	m_id = id;
	m_texture = texture;
	m_zone = zone;
}


sf::Rect<float> TilesetSelect::getZone()
{
	return m_zone;
}

sf::Vector2f TilesetSelect::getMaxZonePos()
{
	return sf::Vector2f(m_texture->getSize().x - m_zone.width, m_texture->getSize().y - m_zone.height);
}

void TilesetSelect::setPosition(sf::Vector2f position)
{
	m_position = position;
}


void TilesetSelect::setSize(sf::Vector2f size)
{
	m_size = size;
}


void TilesetSelect::setZone(sf::Rect<float> zone)
{
	sf::Rect<float> tmp_zone = zone;
	if(tmp_zone.left + tmp_zone.width > m_texture->getSize().x)
		tmp_zone.left = m_texture->getSize().x - tmp_zone.width;

	if(tmp_zone.top + tmp_zone.height > m_texture->getSize().y)
		tmp_zone.top = m_texture->getSize().y - tmp_zone.height;

	if(tmp_zone.left < 0)
	{
		tmp_zone.left = 0;
		tmp_zone.width = m_texture->getSize().x;
	}

	if(tmp_zone.top < 0)
	{
		tmp_zone.top = 0;
		tmp_zone.height = m_texture->getSize().y;
	}

	m_zone = tmp_zone;
}


void TilesetSelect::interactsWithUser(sf::RenderWindow* window)
{

}


void TilesetSelect::update()
{
	m_vertex.setPrimitiveType(sf::Quads);

	m_vertex.resize(4);

	m_vertex[0].position = m_position;
	m_vertex[1].position = m_position + sf::Vector2f( m_zone.width, 0);
	m_vertex[2].position = m_position + sf::Vector2f( m_zone.width, m_zone.height );
	m_vertex[3].position = m_position + sf::Vector2f( 0, m_zone.height );


	m_vertex[0].texCoords = sf::Vector2f(m_zone.left, m_zone.top);
	m_vertex[1].texCoords = sf::Vector2f(m_zone.left + m_zone.width , m_zone.top);
	m_vertex[2].texCoords = sf::Vector2f(m_zone.left + m_zone.width, m_zone.top + m_zone.height);
	m_vertex[3].texCoords = sf::Vector2f(m_zone.left, m_zone.top + m_zone.height);
}


void TilesetSelect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = m_texture;

	target.draw(m_vertex, states);
}




//////////////////////////////////////////////////////////// SLIDEBAR //////////////////////////////////////////////////////////



SlideBar::SlideBar()
{

}


SlideBar::SlideBar(std::string id, float minValue, float maxValue)
{
	m_id = id;
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_currentValue = minValue;
}


SlideBar::~SlideBar()
{

}


// Accesseurs
float SlideBar::getMinValue()
{
	return m_minValue;
}

float SlideBar::getMaxValue()
{
	return m_maxValue;
}

float SlideBar::getCurrentValue()
{
	return m_currentValue;
}

SlideBar::Type SlideBar::getType()
{
	return m_type;
}


//Modificateurs
void SlideBar::setMinValue(float minValue)
{
	m_minValue = minValue;
}

void SlideBar::setMaxValue(float maxValue)
{
	m_maxValue = maxValue;
}

void SlideBar::setCurrentValue(float currentValue)
{
	m_currentValue = currentValue;
}

void SlideBar::setCurrentValueFromPos(sf::Vector2f clickPos)
{
	if(this->getType()==HORIZONTAL)
	{
		this->setCurrentValue( this->getMinValue() + (this->getMaxValue() - this->getMinValue())*(clickPos.x - this->getPosition().x)/this->getSize().x );
		this->update();
	}
	else
	{
		this->setCurrentValue( this->getMinValue() + (this->getMaxValue() - this->getMinValue())*(clickPos.y - this->getPosition().y)/this->getSize().y );
		this->update();
	}
}

void SlideBar::setType(Type type)
{
	m_type = type;
}



//Méthodes
void SlideBar::interactsWithUser(sf::RenderWindow* window)
{
	// Interaction avec l'utilisateur
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect((sf::Vector2i)this->getPosition(), (sf::Vector2i)this->getSize()).contains(mousePos.x, mousePos.y))
		m_state = HOVER;
	else if(m_state==HOVER && sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		m_state = CLICK;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_state = NONE;

	if(m_state == CLICK)
	{
		this->setCurrentValueFromPos(mousePos);
	}

	this->update();
}

void SlideBar::update()
{
	if(m_currentValue > m_maxValue)	
		m_currentValue = m_maxValue;

	if(m_currentValue < m_minValue)	
		m_currentValue = m_minValue;

}


void SlideBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape zone_bar, sliding_bar;
	zone_bar.setFillColor(sf::Color(50,50,50,180));
	zone_bar.setOutlineThickness(2);
	zone_bar.setOutlineColor(sf::Color(40,40,40,200));
	zone_bar.setSize(m_size);
	zone_bar.setPosition(m_position);

	sliding_bar.setFillColor(sf::Color(150, 150, 150, 225));

	if(m_type==HORIZONTAL)
	{
		sliding_bar.setSize(sf::Vector2f(m_size.x*0.5, m_size.y));
		sliding_bar.setPosition(m_position.x + (m_size.x - sliding_bar.getSize().x)*(m_currentValue - m_minValue)/(m_maxValue - m_minValue), m_position.y);
	}
	else
	{
		sliding_bar.setSize(sf::Vector2f(m_size.x, m_size.y*0.5));
		sliding_bar.setPosition(m_position.x , m_position.y + (m_size.y - sliding_bar.getSize().y)*(m_currentValue - m_minValue)/(m_maxValue - m_minValue));
	}

	target.draw(zone_bar, states);
	target.draw(sliding_bar, states);
}





///////////////////////////////////////////////////////////// TAB //////////////////////////////////////////////////////////////


Tab::Tab()
{

}

Tab::~Tab()
{
	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		delete (it->second);
		m_objects.erase(it);
	}
}


Tab::Tab(std::string name_title, sf::RenderWindow* window, void (*function)(void*))
{
	initialized = false;

	m_window = window;
	tabFunction = function;
	m_title = name_title;
	m_title_size = 30;

	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	else
	{
		sf::Text tmp_char(m_title, m_font, BOXWINDOW_SIZE_CHARACTER);		
		m_title_size = tmp_char.getLocalBounds().height * 2;
	}
}


//Accesseurs
sf::Vector2f Tab::getPosition()
{
	return m_position;
}


sf::Vector2f Tab::getSize()
{
	return m_size;
}


float Tab::getTitleSize()
{
	return m_title_size;
}


float Tab::getXOffset()
{
	return m_x_offset_title_pos;
}


std::string	Tab::getTitle()
{
	return m_title;
}


sf::RenderWindow* Tab::getRenderWindow()
{
	return m_window;
}


InterfaceObject* Tab::getObject(std::string objectID)
{
	std::map<std::string, InterfaceObject*>::iterator tmp_it;
	if( (tmp_it=m_objects.find(objectID)) != m_objects.end())
		return tmp_it->second;
	else
		return NULL;
}



//Modificateurs
void Tab::setSize(sf::Vector2f size)
{
	m_size=size;
}


void Tab::setTitleSize(float title_size)
{
	m_title_size = title_size;
}


void Tab::setPosition(sf::Vector2f position)
{
	m_position = position;
}

void Tab::setXOffset(float offset)
{
	m_x_offset_title_pos = offset;
}

void Tab::setTitle(std::string title)
{
	m_title = title;
}




//Méthodes
void Tab::Function(void* arg)
{
	tabFunction(arg);
}


void Tab::addObject(InterfaceObject* object)
{
	m_objects.insert( std::pair<std::string, InterfaceObject*>(object->getID(),object) );
}


void Tab::update()
{
	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		(it->second)->update();
	}
}

void Tab::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Text title_char(m_title, m_font, BOXWINDOW_SIZE_CHARACTER);
	title_char.setOutlineThickness(1);
	title_char.setOutlineColor(sf::Color::Black);


	sf::RectangleShape titlebar(sf::Vector2f(title_char.getLocalBounds().width * 1.3, m_title_size));
	sf::RectangleShape appearance(sf::Vector2f(m_size.x, m_size.y-titlebar.getSize().y));
	titlebar.setFillColor(sf::Color(50, 75, 135, 200));
	appearance.setFillColor(sf::Color(100, 100 , 100 , 160));

	titlebar.setPosition(m_position + sf::Vector2f(m_x_offset_title_pos, 0));
	appearance.setPosition(m_position + sf::Vector2f(0, titlebar.getSize().y) );
	title_char.setPosition(titlebar.getPosition() + sf::Vector2f(titlebar.getSize().x/2 - title_char.getLocalBounds().width/2, titlebar.getSize().y/2 - title_char.getLocalBounds().height/2 - 2));


	target.draw(appearance, states);
	target.draw(titlebar);
	target.draw(title_char);


	for(std::map<std::string, InterfaceObject*>::const_iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		target.draw(*it->second);
	}
}





/////////////////////////////////////////////////////////// BOXWINDOW //////////////////////////////////////////////////////////


BoxWindow::BoxWindow()
{

}


BoxWindow::BoxWindow(sf::Vector2f size, sf::RenderWindow* window)
{
	m_size = size;
	m_window = window;
	m_minSize = sf::Vector2f(200, 200);
	m_maxSize = sf::Vector2f(800, 800);
	m_tabtitle_size = 30;

	//Taille en fonction de la police
	sf::Font tmp_font;
	if(!tmp_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	else
	{
		sf::Text tmp_char("//", tmp_font, BOXWINDOW_SIZE_CHARACTER);		
		m_tabtitle_size = tmp_char.getLocalBounds().height * 2;
	}
}



BoxWindow::~BoxWindow()
{

}

//Accesseurs
Tab* BoxWindow::getTab(int index)
{
	int i = 0;
	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		if(i==index) return *it;
		i++;
	}

	return NULL;
}


sf::Vector2f BoxWindow::getPosition()
{
	return m_position;
}


sf::Vector2f BoxWindow::getSize()
{
	return m_size;
}


sf::Vector2f BoxWindow::getMinSize()
{
	return m_minSize;
}


sf::Vector2f BoxWindow::getMaxSize()
{
	return m_maxSize;
}


float BoxWindow::getTabTitleSize()
{
	return m_tabtitle_size;
}






//Modificateurs
void BoxWindow::setSize(sf::Vector2f size)
{
	m_size=size;
}


void BoxWindow::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_position_window = m_position - sf::Vector2f(m_window->getView().getCenter().x - m_window->getView().getSize().x/2, m_window->getView().getCenter().y - m_window->getView().getSize().y/2);
}

void BoxWindow::setPositionWindow(sf::Vector2f position)
{
	// Il s'agit de la position relative à la View de la RenderWindow prise en paramètre et non des coordonnées absolues
	m_position_window = position;
	m_position = sf::Vector2f(m_window->getView().getCenter().x - m_window->getView().getSize().x/2, m_window->getView().getCenter().y - m_window->getView().getSize().y/2) + m_position_window;
}



void BoxWindow::setMinSize(sf::Vector2f minSize)
{
	m_minSize = minSize;
}


void BoxWindow::setMaxSize(sf::Vector2f maxSize)
{
	m_minSize = maxSize;
}


void BoxWindow::setTabTitleSize(float tabtitle_size)
{
	m_tabtitle_size = tabtitle_size;
}


//Méthodes
void BoxWindow::addTab(Tab *tab)
{
	m_tabs.push_back(tab);
}



void BoxWindow::move_resize()
{
	// Permet de modifier la taille de la fenêtre et de la déplacer
	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
	static sf::Vector2f click_move_window;
	
	if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(-5, this->getTabTitleSize())   ,   sf::Vector2i(11, this->getSize().y - this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = LEFT;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(this->getSize().x - 5 , this->getTabTitleSize())   ,   sf::Vector2i(11, this->getSize().y - this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = RIGHT;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(0, this->getSize().y - 5)   ,   sf::Vector2i(this->getSize().x, 11)  ).contains(mousePos.x, mousePos.y))
		m_state_hover = BOTTOM;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition(), sf::Vector2i(this->getSize().x, this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = TOP;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_state_hover = NONE;
		m_state_click = NONE;
	}

	//Detection de clic (changement d'état)
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
	{
		if(m_state_hover == LEFT) m_state_click = LEFT; 
		if(m_state_hover == RIGHT) m_state_click = RIGHT;
		if(m_state_hover == BOTTOM) m_state_click = BOTTOM;
		if(m_state_hover == TOP) m_state_click = TOP;

		m_state_hover = NONE;
	}
	else
		m_state_click = NONE;

	if(m_state_click==NONE) click_move_window = mousePos - this->getPosition(); // Permet de déplacer la fenêtre depuis le point d'accroche



	if(m_state_click == LEFT && this->getSize().x + this->getPosition().x - mousePos.x > this->getMinSize().x && this->getSize().x + this->getPosition().x - mousePos.x < this->getMaxSize().x) 
	{ 
		this->setSize(sf::Vector2f(this->getSize().x + this->getPosition().x - mousePos.x, this->getSize().y));
		this->setPosition(sf::Vector2f(mousePos.x,this->getPosition().y));
	}
	else if(m_state_click == RIGHT   &&   mousePos.x - this->getPosition().x > this->getMinSize().x   &&   mousePos.x - this->getPosition().x < this->getMaxSize().x) 
	{ 
		this->setSize(sf::Vector2f(mousePos.x - this->getPosition().x, this->getSize().y));
	}
	else if(m_state_click == BOTTOM   &&   mousePos.y - this->getPosition().y > this->getMinSize().y   &&   mousePos.y - this->getPosition().y < this->getMaxSize().y) 
	{ 
		this->setSize(sf::Vector2f(this->getSize().x, mousePos.y - this->getPosition().y));
	}
	else if(m_state_click == TOP)
	{
		this->setPosition(mousePos - click_move_window);
	}

	this->update();
}


void BoxWindow::interactsWithUser()
{
	this->move_resize();
}



void BoxWindow::update()
{
	this->setPositionWindow(m_position_window);
	int i=0;
	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		(*it)->setSize(m_size);
		(*it)->setTitleSize(m_tabtitle_size);
		(*it)->setPosition(m_position);
		(*it)->setXOffset(i * (*it)->getTitle().length() * BOXWINDOW_SIZE_CHARACTER);
		(*it)->update();
	}
}

void BoxWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape appearance(m_size);
	appearance.setPosition(m_position);
	appearance.setFillColor(sf::Color(50, 75, 135, 200));
	target.draw(appearance);
	for(std::vector<Tab*>::const_iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		target.draw(*(*it), states);
	}
}




