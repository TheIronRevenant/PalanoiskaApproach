#include "../globals.hpp"
#include "mesh.hpp"

Mesh::Mesh(int gridx, int gridy, int gridw, int gridh) {
	boundingBox = sf::RectangleShape(sf::Vector2f((float)gridw * Globals::TileSize, (float)gridh * Globals::TileSize));
	boundingBox.setPosition((float)gridx * Globals::TileSize, (float)gridy * Globals::TileSize);
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Red);
	boundingBox.setFillColor(sf::Color::Transparent);
}

void Mesh::draw(sf::RenderWindow& window) {
	window.draw(boundingBox);
}

void Mesh::expandRight() {
	sf::Vector2f size = boundingBox.getSize();
	size.x += Globals::TileSize;
	boundingBox.setSize(size);
}

void Mesh::expandDown() {
	sf::Vector2f size = boundingBox.getSize();
	size.y += Globals::TileSize;
	boundingBox.setSize(size);
}