#include "dialogue.hpp"

Dialogue::Dialogue(float x, float y, std::string text, unsigned int fontSize, const sf::Font& font) {
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(fontSize);
	this->text.setFillColor(sf::Color::Black);
	this->text.setPosition(x, y);
}

void Dialogue::draw(sf::RenderWindow& window) {
	window.draw(text);
}