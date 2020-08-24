#include "dialogue.hpp"

Dialogue::Dialogue(float x, float y, std::string text, unsigned int fontSize, const sf::Font& font) {
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(fontSize);
	this->text.setFillColor(sf::Color::Black);
	this->text.setPosition((int)x, (int)y);
	this->text.setOutlineColor(sf::Color::White);
	this->text.setOutlineThickness(0.4f);
}

void Dialogue::draw(sf::RenderWindow& window) {
	window.draw(text);
}