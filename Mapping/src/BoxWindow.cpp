#include "BoxWindow.h"
#include "functionTabs.h"

#define FONT_FILE "AldoTheApache.ttf"
#define WIDTH_TITLETAB_COEFF 1.3


int BOXWINDOW_SIZE_CHARACTER = 15;


BoxWindow* BOXWINDOW_FOCUS_WINDOW;


void unFocus_boxwindow() { BOXWINDOW_FOCUS_WINDOW=NULL; }
bool noFocus_boxwindow() { return BOXWINDOW_FOCUS_WINDOW==NULL; }


///////////////////////////////////////////////////////////// TAB //////////////////////////////////////////////////////////////


Tab::Tab()
{

}

Tab::~Tab()
{
	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		delete it->second;
		m_objects.erase(it);
	}


	if(m_function_arg != NULL)
		free(m_function_arg);
}


Tab::Tab(std::string name_title, sf::RenderWindow* window, void (*function)(Tab*, ArgTab*), ArgTab* function_arg, size_t size_function_arg)
{
	//Pointers association and allocation
	m_window = window;
	tabFunction = function;

	if(function_arg != NULL)
	{
		m_function_arg = (ArgTab*)malloc(size_function_arg);
		*m_function_arg = *function_arg;
	}
	else
		m_function_arg = NULL;


	// Variable intialization
	initialized = false;
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
void Tab::interactsWithUser(sf::RenderWindow* window)
{
	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		(it->second)->interactsWithUser(window);
	}
}



void Tab::Function()
{
	/* Execute the function tabFunction assiociated to the tab with its argument */

	if(tabFunction != NULL)
		tabFunction(this, m_function_arg);
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
	sf::RectangleShape appearance(sf::Vector2f(m_size.x, m_size.y-m_title_size));
	appearance.setFillColor(sf::Color(100, 100 , 100 , 160));
	appearance.setPosition(m_position + sf::Vector2f(0, m_title_size) );

	target.draw(appearance, states);


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
	if(!m_font.loadFromFile(FONT_FILE)) std::cerr << "Erreur lors du chargement de la police" << std::endl;
	else
	{
		sf::Text tmp_char("//", m_font, BOXWINDOW_SIZE_CHARACTER);		
		m_tabtitle_size = tmp_char.getLocalBounds().height * 2;
	}
}



BoxWindow::~BoxWindow()
{
	for (int i = m_tabs.size() - 1 ; i >= 0 ; i--)
	{
		delete m_tabs[i];
		m_tabs.erase(m_tabs.begin()+i);
	}
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
	if(minSize.x >= m_maxSize.x) m_minSize.x = m_maxSize.x - 1;
	else m_minSize.x = minSize.x;
	if(minSize.y >= m_maxSize.y) m_minSize.y = m_maxSize.y - 1;
	else m_minSize.y = minSize.y;

	if(this->getSize().x < m_minSize.x) this->setSize(sf::Vector2f(m_minSize.x, this->getSize().y));
	if(this->getSize().y < m_minSize.y) this->setSize(sf::Vector2f(this->getSize().x, m_minSize.y));

}


void BoxWindow::setMaxSize(sf::Vector2f maxSize)
{
	if(maxSize.x <= m_minSize.x) m_maxSize.x = m_minSize.x + 1;
	else m_maxSize.x = maxSize.x;
	if(maxSize.y <= m_minSize.y) m_maxSize.y = m_minSize.y + 1;
	else m_maxSize.y = maxSize.y;


	if(this->getSize().x > m_maxSize.x) this->setSize(sf::Vector2f(m_maxSize.x, this->getSize().y));
	if(this->getSize().y > m_maxSize.y) this->setSize(sf::Vector2f(this->getSize().x, m_maxSize.y));
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



void BoxWindow::setFocus()
{
	BOXWINDOW_FOCUS_WINDOW = this;
}

bool BoxWindow::hasFocus()
{
	return this==BOXWINDOW_FOCUS_WINDOW;
}


void BoxWindow::focusTab(Tab* tab)
{
	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end() ; it++)
	{
		if(*it == tab)
		{
			m_focus_tab = tab;
			break;
		}
	}

}


void BoxWindow::move_resize()
{
	// Permet de modifier la taille de la fenêtre, de la déplacer et de choisir un onglet

	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
	
	if(this->hasFocus() && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(-5, this->getTabTitleSize())   ,   sf::Vector2i(11, this->getSize().y - this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = LEFT;
	else if(this->hasFocus() && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(this->getSize().x - 5 , this->getTabTitleSize())   ,   sf::Vector2i(11, this->getSize().y - this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = RIGHT;
	else if(this->hasFocus() && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(0, this->getSize().y - 5)   ,   sf::Vector2i(this->getSize().x, 11)  ).contains(mousePos.x, mousePos.y))
		m_state_hover = BOTTOM;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition(), sf::Vector2i(this->getSize().x, this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = TOP;
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_state_hover = NONE;
		m_state_click = NONE;
	}


	// Detection de clic (changement d'état)

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
	{
		if(m_state_hover == LEFT) m_state_click = LEFT; 
		if(m_state_hover == RIGHT) m_state_click = RIGHT;
		if(m_state_hover == BOTTOM) m_state_click = BOTTOM;
		if(m_state_hover == TOP) 
		{
			// Permet de changer l'onglet sur lequel la fenêtre a son focus
			if(this->hasFocus())
			{
				for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++)
				{
					sf::Text tmp_char((*it)->getTitle(), m_font, BOXWINDOW_SIZE_CHARACTER);
					sf::Rect<float> rect_title_tab(m_position + sf::Vector2f((*it)->getXOffset(), 0), sf::Vector2f(tmp_char.getLocalBounds().width * WIDTH_TITLETAB_COEFF, (*it)->getTitleSize()));	

					if(rect_title_tab.contains(mousePos))
					{
						this->focusTab(*it);
						break;
					}
				}
			}

			m_state_click = TOP;
		}

		m_state_hover = NONE;
	}
	else
		m_state_click = NONE;



	if(m_state_click==NONE) m_click_move_window = mousePos - this->getPosition(); // Permet de déplacer la fenêtre depuis le point d'accroche



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
	else if(this->hasFocus() && m_state_click == TOP)
	{
		this->setPosition(mousePos - m_click_move_window);
	}

	this->update();
}




void BoxWindow::interactsWithUser()
{
	this->move_resize();
	if((this->hasFocus() || BOXWINDOW_FOCUS_WINDOW==NULL) && m_focus_tab!=NULL)
		m_focus_tab->interactsWithUser(m_window);
}




void BoxWindow::Function()
{
	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		(*it)->Function();
	}
}





void BoxWindow::update()
{
	/* Update the boxwindow and the tabs */

	this->setPositionWindow(m_position_window);
	int i=0;
	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		(*it)->setSize(m_size);
		(*it)->setTitleSize(m_tabtitle_size);
		(*it)->setPosition(m_position);
		(*it)->setXOffset(i);
		(*it)->update();

		sf::Text tmp_char((*it)->getTitle(), m_font, BOXWINDOW_SIZE_CHARACTER);
		i+=tmp_char.getLocalBounds().width * WIDTH_TITLETAB_COEFF;

	}

	if(this->hasFocus())
		m_focus = true;
	else
		m_focus = false;
}

void BoxWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Draw a halo around the window if it has focus
	if(m_focus)
	{
		sf::RectangleShape focus(m_size);
		focus.setPosition(m_position);
		focus.setFillColor(sf::Color(0,0,0,0));
		focus.setOutlineColor(sf::Color(255, 255, 255, 200));
		focus.setOutlineThickness(1);

		target.draw(focus, states);
	}


	// Draw the general window appearance (the window bloc background)
	sf::RectangleShape appearance(m_size);
	appearance.setPosition(m_position);
	appearance.setFillColor(sf::Color(50, 75, 135, 200));
	target.draw(appearance);



	// Draw the title for each tab
	for(std::vector<Tab*>::const_iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		if((*it)!=m_focus_tab)
		{
			sf::Text title_char((*it)->getTitle(), m_font, BOXWINDOW_SIZE_CHARACTER);
			sf::RectangleShape titlebar(sf::Vector2f(title_char.getLocalBounds().width * WIDTH_TITLETAB_COEFF, (*it)->getTitleSize()));

			title_char.setOutlineThickness(1);
			title_char.setOutlineColor(sf::Color::Black);
			
			titlebar.setFillColor(sf::Color(20, 45, 85, 200));


			titlebar.setPosition(m_position + sf::Vector2f((*it)->getXOffset(), 0));
			title_char.setPosition((int)(titlebar.getPosition().x + titlebar.getSize().x/2 - title_char.getLocalBounds().width/2), (int)(titlebar.getPosition().y + titlebar.getSize().y/2 - title_char.getLocalBounds().height/2 - 2));

			target.draw(titlebar, states);
			target.draw(title_char, states);
		}
	}


	// Draw the focused tab
	if(m_focus_tab!=NULL)
	{
		sf::Text title_char(m_focus_tab->getTitle(), m_font, BOXWINDOW_SIZE_CHARACTER);
		sf::RectangleShape titlebar(sf::Vector2f(title_char.getLocalBounds().width * WIDTH_TITLETAB_COEFF, m_focus_tab->getTitleSize()));

		title_char.setOutlineThickness(1);
		title_char.setOutlineColor(sf::Color::Black);
		
		titlebar.setFillColor(sf::Color(50, 75, 135, 200));

		titlebar.setPosition(m_position + sf::Vector2f(m_focus_tab->getXOffset(), 0));
		title_char.setPosition((int)(titlebar.getPosition().x + titlebar.getSize().x/2 - title_char.getLocalBounds().width/2), (int)(titlebar.getPosition().y + titlebar.getSize().y/2 - title_char.getLocalBounds().height/2 - 2));

		target.draw(titlebar, states);
		target.draw(title_char, states);
		target.draw(*m_focus_tab, states);
	}

}




