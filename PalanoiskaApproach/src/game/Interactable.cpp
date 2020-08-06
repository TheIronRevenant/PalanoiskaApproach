#include "interactable.hpp"
#include "../engine/game.hpp"
#include "../globals.hpp"
#include <iostream>

Interactable::Interactable(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Green);
	boundingBox.setFillColor(sf::Color::Transparent);
}

void Interactable::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}