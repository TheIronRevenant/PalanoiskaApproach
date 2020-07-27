#include <algorithm>
#include "../globals.hpp"
#include "gameobject.hpp"

/*
Game Object Abstract Class
*/

#pragma warning(disable : 26812 ) //Disable warning about sfml

GameObject::GameObject(unsigned int gridx, unsigned int gridy, sf::Texture& texture) {
	boundingBox = sf::RectangleShape(sf::Vector2f(texture.getSize()));
	boundingBox.setPosition((float)gridx * Globals::TileSize, (float)gridy * Globals::TileSize);
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Red);
	boundingBox.setFillColor(sf::Color::Transparent);

	sprite = sf::Sprite();
	sprite.setPosition(getPosition());
	sprite.setTexture(texture);
}

#pragma warning(default : 26812 )

void GameObject::draw(sf::RenderWindow& window) const {
	window.draw(sprite);
}

void GameObject::draw(sf::RenderTexture& texture) const {
	texture.draw(sprite);
}

void GameObject::move(sf::Vector2f vector) {
	sf::Vector2f pos = vector + getPosition();
	boundingBox.setPosition(pos);
	sprite.setPosition(pos);
}

void GameObject::move(float x, float y) {
	sf::Vector2f pos = getPosition();
	pos.x += x;
	pos.y += y;
	boundingBox.setPosition(pos);
	sprite.setPosition(pos);
}

void GameObject::setPosition(sf::Vector2f vector) {
	boundingBox.setPosition(vector);
	sprite.setPosition(getPosition());
}

void GameObject::setPosition(float x, float y) {
	boundingBox.setPosition(x, y);
	sprite.setPosition(getPosition());
}