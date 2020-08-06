#include "interactable.hpp"

Interactable::Interactable(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {
	boundingBox.setOutlineThickness(0.5f);
	boundingBox.setOutlineColor(sf::Color::Green);
	boundingBox.setFillColor(sf::Color::Transparent);
}