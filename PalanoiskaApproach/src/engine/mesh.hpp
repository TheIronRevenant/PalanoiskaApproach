#ifndef MESH_H
#define MESH_H

#include <SFML/Graphics.hpp>

class Mesh {
public:
	Mesh() {}
	Mesh(int gridx, int gridy, int gridw, int gridh);
	void draw(sf::RenderWindow& window);
	void expandRight();
	void expandDown();
	const sf::RectangleShape& getBoundingBox() const { return boundingBox; }
private:
	sf::RectangleShape boundingBox;
};

#endif