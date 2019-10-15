#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <SFML/Graphics.hpp>

class Camera
{
public:
	//Constructors
	Camera();
	Camera(sf::RenderWindow* window, bool progressive_movement, float speed, sf::Vector2f focus_point, sf::Vector2f size_view);
	~Camera();


	// Getters
	sf::Vector2f 	getFocusPoint();
	sf::Vector2f 	getSizeView();
	sf::View 		getView();
	sf::FloatRect 	getCameraRect();

	// Setters
	void 			setFocusPoint(sf::Vector2f focus_point);
	void 			setSizeView(sf::Vector2f zoom);


	// Methods
	void 			update();


private:
	sf::RenderWindow* 				m_window;
	sf::View 						m_view;
	sf::Vector2f 					m_focus_point;
	sf::Vector2f 					m_size_view;				

	bool 							m_progressive_movement;
	float 							m_speed;
	float 							m_limit_scope;
};












#endif