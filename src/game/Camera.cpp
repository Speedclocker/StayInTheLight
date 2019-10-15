#include "Camera.h"
#include "cmath"


Camera::Camera()
{

}


Camera::~Camera()
{

}


Camera::Camera(sf::RenderWindow* window, bool progressive_movement, float speed, sf::Vector2f focus_point, sf::Vector2f size_view)
{
	m_window = window;
	m_progressive_movement = progressive_movement;
	m_speed = speed;
	m_focus_point = focus_point;
	m_size_view = size_view;

	this->update();
}




// Getters

sf::Vector2f Camera::getFocusPoint()
{
	return m_focus_point;
}


sf::Vector2f Camera::getSizeView()
{
	return m_size_view;
}


sf::View Camera::getView()
{
	return m_view;
}


sf::FloatRect Camera::getCameraRect()
{
	return sf::FloatRect(this->getView().getCenter() - sf::Vector2f(this->getView().getSize().x/2, this->getView().getSize().y/2), this->getView().getSize());
}



// Setters

void Camera::setFocusPoint(sf::Vector2f focus_point)
{
	m_focus_point = focus_point;
}


void Camera::setSizeView(sf::Vector2f size_view)
{
	m_size_view = size_view;
}



// Methods

void Camera::update()
{
	if(m_view.getSize().x != m_size_view.x || m_view.getSize().y != m_size_view.y)
		m_view.setSize(m_size_view);

	if(!m_progressive_movement)
		m_view.setCenter(m_focus_point);
	else
	{
		m_view.move((m_focus_point - m_view.getCenter())*0.01f*m_speed);
	}

	m_window->setView(m_view);
}

