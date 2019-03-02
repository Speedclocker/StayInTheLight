#include <SFML/Graphics.hpp>
#include <iostream>

typedef struct
{
	sf::Texture*	m_texture;
	sf::Vector2f	m_pos_text;
	sf::Vector2f	m_size_text;
	bool 		m_collisionable;
} Tile ;

class Map : public sf::Drawable, public sf::Transformable
{
public:
	Map(void);
	Map(sf::Vector2f size, int height);

	int getHeight() const;
	sf::Vector2f getSize() const;

	void update();
	
		
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	

	Tile*** m_map;
	sf::VertexArray m_vertex;
	int m_height;
	sf::Vector2f m_size;
	
};

