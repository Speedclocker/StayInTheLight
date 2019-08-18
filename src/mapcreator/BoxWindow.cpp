#include "BoxWindow.h"
#include "functionTabs.h"

#define FONT_FILE "data/fonts/AldoTheApache.ttf"
#define WIDTH_TITLETAB_COEFF 1.3
#define BOXWINDOW_SIZE_CHARACTER 16





///////////////////////////////////////////////////////////// Tab //////////////////////////////////////////////////////////////


// Constructors / Destructors

Tab::Tab()
{
	/* Tab Constructor */
}

Tab::~Tab()
{
	/* Tab Destructor */

	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		delete it->second;
	}
	m_objects.clear();


	if(m_function_arg != NULL)
		free(m_function_arg);
}


Tab::Tab(std::string name_title, sf::RenderWindow* window, void (*function)(Tab*, ArgTab*), ArgTab* function_arg, size_t size_function_arg)
{
	/* Tab Constructor */
	/* Take in parameters the name of the tab (title name), the pointer to the window, the pointer to the function associated to the tab, the pointer to the argument and the size of the argument */

	//Pointers association and allocation
	m_window = window;
	tabFunction = function;
	m_focus_object = NULL;

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




// Getters

sf::Vector2f Tab::getPosition()
{
	/* Return the position of the tab */

	return m_position;
}


sf::Vector2f Tab::getSize()
{
	/* Return the size of the tab */

	return m_size;
}


float Tab::getTitleSize()
{
	/* Return the size of the tab (height) */

	return m_title_size;
}


float Tab::getXOffset()
{
	/* Return the offset from the position, used to place the title name */

	return m_x_offset_title_pos;
}


std::string	Tab::getTitle()
{
	/* Return the title string */

	return m_title;
}


sf::RenderWindow* Tab::getRenderWindow()
{
	/* Return the window associated to the tab */

	return m_window;
}


InterfaceObject* Tab::getObject(std::string objectID)
{
	/* Return the object according to its name taken for argument */ 

	std::map<std::string, InterfaceObject*>::iterator tmp_it;
	if( (tmp_it=m_objects.find(objectID)) != m_objects.end())
		return tmp_it->second;
	else
		return NULL;
}



// Setters

void Tab::setSize(sf::Vector2f size)
{
	/* Set size */

	m_size=size;
}


void Tab::setTitleSize(float title_size)
{
	/* Set Title Size */

	m_title_size = title_size;
}


void Tab::setPosition(sf::Vector2f position)
{
	/* Set Position */

	m_position = position;
}

void Tab::setXOffset(float offset)
{
	/* Set the offset in x coords from the position */

	m_x_offset_title_pos = offset;
}

void Tab::setTitle(std::string title)
{
	/* Set the title name */

	m_title = title;
}




// Methods

void Tab::interactsWithUser(sf::RenderWindow* window)
{
	/* Allow to interact with the user */

	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{			
		it->second->interactsWithUser(window);	
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
	/* Add an interface object to the tab */

	m_objects.insert( std::pair<std::string, InterfaceObject*>(object->getID(),object) );
}


void Tab::update()
{
	/* Update the tab */

	for(std::map<std::string, InterfaceObject*>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		(it->second)->update();
	}
}

void Tab::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the tab */

	sf::RectangleShape appearance(sf::Vector2f(m_size.x, m_size.y-m_title_size));
	appearance.setFillColor(sf::Color(100, 100 , 100 , 160));
	appearance.setPosition(m_position + sf::Vector2f(0, m_title_size) );

	target.draw(appearance, states);

	std::vector<InterfaceObject*> overall_objects;

	for(std::map<std::string, InterfaceObject*>::const_iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		//if(m_focus_object==NULL || m_focus_object!=it->second)
		if(!it->second->isOverall())
			target.draw(*it->second);
		else
			overall_objects.push_back(it->second);
	}

	for(std::vector<InterfaceObject*>::const_iterator it = overall_objects.begin(); it != overall_objects.end(); it++)
		target.draw(**it);

	//if(m_focus_object!=NULL)
	//	target.draw(*m_focus_object);
}







/////////////////////////////////////////////////////////// BoxWindow //////////////////////////////////////////////////////////



// Constructors / Destructor 

BoxWindow::BoxWindow()
{
	/* Constructor */
}


BoxWindow::BoxWindow(sf::Vector2f size, sf::RenderWindow* window)
{
	/* Constructor */

	m_size = size;
	m_window = window;
	m_minSize = sf::Vector2f(200, 200);
	m_maxSize = sf::Vector2f(800, 800);
	m_tabtitle_size = 30;
	m_focus_tab = NULL;

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
	/* Destructor */

	for (int i = m_tabs.size() - 1 ; i >= 0 ; i--)
	{
		delete m_tabs[i];
	}
	m_tabs.clear();
}



// Getters

Tab* BoxWindow::getTab(int index)
{
	/* Return the pointer to the tab from the index taken for argument */

	int i = 0;
	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		if(i==index) return *it;
		i++;
	}

	return NULL;
}

Tab* BoxWindow::getFocusTab()
{
	return m_focus_tab;
}


sf::Vector2f BoxWindow::getPosition()
{
	/* Return the position of the boxwindow*/

	return m_position;
}


sf::Vector2f BoxWindow::getSize()
{
	/* Return the size of the boxwindow*/

	return m_size;
}


sf::Vector2f BoxWindow::getMinSize()
{
	/* Return the minimum size of the boxwindow*/

	return m_minSize;
}


sf::Vector2f BoxWindow::getMaxSize()
{
	/* Return the maximum size of the boxwindow*/

	return m_maxSize;
}


float BoxWindow::getTabTitleSize()
{
	/* Return the size of the tab header */

	return m_tabtitle_size;
}



// Setters

void BoxWindow::setSize(sf::Vector2f size)
{
	/* Set size */

	m_size=size;
}


void BoxWindow::setPosition(sf::Vector2f position)
{
	/* Set position */

	m_position = position;
	m_position_window = m_position - sf::Vector2f(m_window->getView().getCenter().x - m_window->getView().getSize().x/2, m_window->getView().getCenter().y - m_window->getView().getSize().y/2);
}

void BoxWindow::setPositionWindow(sf::Vector2f position)
{
	/* Set position using the window view as reference */

	m_position_window = position;
	m_position = sf::Vector2f(m_window->getView().getCenter().x - m_window->getView().getSize().x/2, m_window->getView().getCenter().y - m_window->getView().getSize().y/2) + m_position_window;
}



void BoxWindow::setMinSize(sf::Vector2f minSize)
{
	/* Set the minimum size possible for resizing the window */

	if(minSize.x >= m_maxSize.x) m_minSize.x = m_maxSize.x - 1;
	else m_minSize.x = minSize.x;
	if(minSize.y >= m_maxSize.y) m_minSize.y = m_maxSize.y - 1;
	else m_minSize.y = minSize.y;

	if(this->getSize().x < m_minSize.x) this->setSize(sf::Vector2f(m_minSize.x, this->getSize().y));
	if(this->getSize().y < m_minSize.y) this->setSize(sf::Vector2f(this->getSize().x, m_minSize.y));

}


void BoxWindow::setMaxSize(sf::Vector2f maxSize)
{
	/* Set the maximum size possible for resizing the window */

	if(maxSize.x <= m_minSize.x) m_maxSize.x = m_minSize.x + 1;
	else m_maxSize.x = maxSize.x;
	if(maxSize.y <= m_minSize.y) m_maxSize.y = m_minSize.y + 1;
	else m_maxSize.y = maxSize.y;


	if(this->getSize().x > m_maxSize.x) this->setSize(sf::Vector2f(m_maxSize.x, this->getSize().y));
	if(this->getSize().y > m_maxSize.y) this->setSize(sf::Vector2f(this->getSize().x, m_maxSize.y));
}


void BoxWindow::setTabTitleSize(float tabtitle_size)
{
	/* Set the size of the tab header */

	m_tabtitle_size = tabtitle_size;
}



// Methods

void BoxWindow::addTab(Tab *tab)
{
	/* Add a tab to the boxwindow from the pointer to the tab taken as argument */

	m_tabs.push_back(tab);
}

void BoxWindow::newTab(std::string tab_name, void (*function)(Tab*, ArgTab*), ArgTab* function_arg, size_t function_arg_size)
{
	/* Create a new tab from a name, a pointer to a function, a pointer to argument and the size of the argument taken as arguments */

	Tab* tab = new Tab(tab_name, m_window, function, function_arg, function_arg_size);
	m_tabs.push_back(tab);
}



void BoxWindow::setInteractable()
{
	/* Make the boxwindow interactable */

	m_interactable = true;
}


void BoxWindow::unInteractable()
{
	/* Make the boxwindow non interactable */

	m_interactable = false;
}

bool BoxWindow::isInteractable()
{
	/* Check if the boxwindow is interactable */

	return m_interactable;
}


void BoxWindow::focusTab(Tab* tab)
{
	/* Focus a tab that pointer is taken as argument */

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
	/* Allow to resize the window and to move it */

	// Permet de modifier la taille de la fenêtre, de la déplacer et de choisir un onglet

	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
	
	if(this->isInteractable() && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(-5, this->getTabTitleSize())   ,   sf::Vector2i(11, this->getSize().y - this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = LEFT;
	else if(this->isInteractable() && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(this->getSize().x - 5 , this->getTabTitleSize())   ,   sf::Vector2i(11, this->getSize().y - this->getTabTitleSize())  ).contains(mousePos.x, mousePos.y))
		m_state_hover = RIGHT;
	else if(this->isInteractable() && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::IntRect( (sf::Vector2i)this->getPosition() + sf::Vector2i(0, this->getSize().y - 5)   ,   sf::Vector2i(this->getSize().x, 11)  ).contains(mousePos.x, mousePos.y))
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
			if(this->isInteractable())
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
	else if(this->isInteractable() && m_state_click == TOP)
	{
		this->setPosition(mousePos - m_click_move_window);
	}

	this->update();
}




void BoxWindow::interactsWithUser()
{
	/* Allow to interact with the user */

	this->move_resize();

	if(this->isInteractable() && m_focus_tab!=NULL)
	{
		m_focus_tab->interactsWithUser(m_window);
	}
}




void BoxWindow::Function()
{
	/* Execute the function assiociated to the different tabs */

	for(std::vector<Tab*>::iterator it = m_tabs.begin(); it != m_tabs.end(); it++ )
	{
		(*it)->Function();
	}
}





void BoxWindow::update()
{
	/* Update the boxwindow and the tabs */

	if(m_focus_tab == NULL && m_tabs.size()>0)
		m_focus_tab = *(m_tabs.begin());

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

	if(this->isInteractable())
		m_interactable = true;
	else
		m_interactable = false;
}

void BoxWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/* Draw the box window */
	
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

			title_char.setOrigin(title_char.getLocalBounds().left, title_char.getLocalBounds().top);

			titlebar.setPosition(m_position + sf::Vector2f((*it)->getXOffset(), 0));
			title_char.setPosition((int)(titlebar.getPosition().x + titlebar.getSize().x/2 - title_char.getLocalBounds().width/2), (int)(titlebar.getPosition().y + titlebar.getSize().y/2 - title_char.getLocalBounds().height/2));

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

		title_char.setOrigin(title_char.getLocalBounds().left, title_char.getLocalBounds().top);

		titlebar.setPosition(m_position + sf::Vector2f(m_focus_tab->getXOffset(), 0));
		title_char.setPosition((int)(titlebar.getPosition().x + titlebar.getSize().x/2 - title_char.getLocalBounds().width/2), (int)(titlebar.getPosition().y + titlebar.getSize().y/2 - title_char.getLocalBounds().height/2));

		target.draw(titlebar, states);
		target.draw(title_char, states);
		target.draw(*m_focus_tab, states);
	}

}





/////////////////////////////////////////////////////////////// BoxWindowsManager ///////////////////////////////////////////////////////////////////////////////:





BWManager::BWManager()
{

}


BWManager::~BWManager()
{
	for(std::map<std::string, BoxWindow*>::iterator it = m_box_windows.begin(); it != m_box_windows.end(); it++)
	{
		delete it->second;
	}

	m_box_windows.clear();
}


BWManager::BWManager(sf::RenderWindow* window)
{
	m_window = window;
	m_focus_window = NULL;
	m_block_on_focus_window = false;
}



// Getters

sf::RenderWindow* BWManager::getWindow(void)
{
	return m_window;
}


BoxWindow* BWManager::getBoxWindow(std::string window_name)
{
	std::map<std::string, BoxWindow*>::iterator tmp_it;
	if( (tmp_it=m_box_windows.find(window_name)) != m_box_windows.end())
		return tmp_it->second;
	else
		return NULL;
}


BoxWindow* BWManager::getFocusBoxWindow()
{
	return m_focus_window;
}



// Setters

void BWManager::setWindow(sf::RenderWindow* window)
{
	m_window = window;
}





// Methods

void BWManager::newBoxWindow(std::string boxwindow_name, sf::Vector2f size)
{
	BoxWindow* new_boxwindow = new BoxWindow(size, m_window);
	m_box_windows.insert( std::pair<std::string, BoxWindow*>(boxwindow_name, new_boxwindow) );
	m_order_box_windows.push_back(new_boxwindow);
}


void BWManager::deleteBoxWindow(std::string boxwindow_name)
{
	std::map<std::string, BoxWindow*>::iterator tmp_it;
	if( (tmp_it=m_box_windows.find(boxwindow_name)) != m_box_windows.end())
	{
		if(m_focus_window == tmp_it->second)
			this->unFocusBoxWindow();
		
		int i = 0;
		bool found = false;
		for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it!=m_order_box_windows.end() && !found; it++)
		{
			if(*it == tmp_it->second)
				found = true;
			else
				i++;
		}

		m_order_box_windows.erase(m_order_box_windows.begin() + i);

		delete tmp_it->second;
		m_box_windows.erase(tmp_it);
	}
}


void BWManager::clearBoxWindows()
{
	for(std::map<std::string, BoxWindow*>::iterator it = m_box_windows.begin(); it != m_box_windows.end(); it++)
	{
		delete it->second;
		m_box_windows.erase(it);
	}
}


void BWManager::interactionsManagement()
{
	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));

	if(!m_block_on_focus_window)
	{
		bool no_BW_act = true; 
		bool focus_change = false;
		for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
		{

			sf::Rect<float> window_rect = sf::Rect<float>((*it)->getPosition() - sf::Vector2f(5,5), (*it)->getSize() + sf::Vector2f(10,10));
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && window_rect.contains(mousePos))
			{
				m_window_hover = (*it);
				no_BW_act = false;
			}
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_window_hover==(*it))
			{
				m_window_click = (*it);
				(*it)->setInteractable();
				m_focus_window = (*it);
				focus_change = true;
				m_window_hover = NULL;
				no_BW_act = false;
			}
			
		}

		if(no_BW_act)
		{
			// If nothing was done on the box windows
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				m_window_hover = NULL;
				m_window_click = NULL;	
			}
			else if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_window_click == NULL)
				this->unFocusBoxWindow();
		}

		if(m_focus_window==NULL)
		{
			for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
			{
				if(*it != m_focus_window)
					(*it)->setInteractable();
			}
		}

		if(focus_change)
		{
			// If the window is focused, put it at the end of the order vector and unfocus all other windows
			
			for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
			{
				if(*it != m_focus_window)
					(*it)->unInteractable();
			}

			std::vector<BoxWindow*>::iterator it = find(m_order_box_windows.begin(), m_order_box_windows.end(), m_focus_window);
			
			m_order_box_windows.erase(it);

			m_order_box_windows.push_back(m_focus_window);
		
		}
	}


	for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
	{
		// Interact and draw in the focus order
		(*it)->interactsWithUser();
		(*it)->Function();	
	}
}


void BWManager::update()
{
	for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
	{
		// Update all windows
		(*it)->update();
	}
}


void BWManager::blockOnFocusWindow()
{
	m_block_on_focus_window = true;
}


void BWManager::unblockOnFocusWindow()
{
	m_block_on_focus_window = false;
}


void BWManager::focusOnBoxWindow(BoxWindow* box_window)
{
	bool found = false;
	for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
	{
		if(box_window == *it)
		{
			m_focus_window = *it;
			(*it)->setInteractable();
			found = true;
		}
	}

	if(found)
	{
		for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
		{
			if(*it != m_focus_window)
				(*it)->unInteractable();
		}

		std::vector<BoxWindow*>::iterator it = find(m_order_box_windows.begin(), m_order_box_windows.end(), m_focus_window);
		
		m_order_box_windows.erase(it);

		m_order_box_windows.push_back(m_focus_window);
	}
}


void BWManager::unFocusBoxWindow()
{
	m_focus_window = NULL;
	m_block_on_focus_window = false;
	for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
		(*it)->unInteractable();
}


void BWManager::drawWindows()
{
	for(std::vector<BoxWindow*>::iterator it = m_order_box_windows.begin(); it != m_order_box_windows.end(); it++)
	{
		// Draw in the focus order
		if(m_focus_window == *it)
		{
			// Darken the window behind the focus window if it's blocked on it
			if(m_block_on_focus_window)
			{
				sf::RectangleShape dark_rect(sf::Vector2f(m_window->getSize().x, m_window->getSize().y));
				dark_rect.setPosition(m_window->getView().getCenter() - sf::Vector2f(m_window->getView().getSize().x/2, m_window->getView().getSize().y/2));
				dark_rect.setFillColor(sf::Color(0,0,0,180));
				m_window->draw(dark_rect);
			}

			// Draw a white rectangle around the focus window
			sf::RectangleShape focus((*it)->getSize());
			focus.setPosition((*it)->getPosition());
			focus.setFillColor(sf::Color(0,0,0,0));
			focus.setOutlineColor(sf::Color(255, 255, 255, 200));
			focus.setOutlineThickness(1);

			m_window->draw(focus);
		}

		m_window->draw(**it);
	}
}


void BWManager::manage()
{
	this->interactionsManagement();
	this->update();
	this->drawWindows();
}