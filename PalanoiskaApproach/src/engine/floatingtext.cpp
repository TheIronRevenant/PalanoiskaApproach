#include "floatingtext.hpp"

FloatingText::FloatingText(float x, float y, std::string text, const sf::Font& font) {
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(16);
	this->text.setFillColor(sf::Color::Black);
	this->text.setOutlineColor(sf::Color::Red);
	this->text.setPosition(x, y);

	riseSpeed = 0.5f;
	riseDuration = 20;
	riseTimer = 0;
}

void FloatingText::update() {
	text.move(0, -riseSpeed);
	riseTimer++;
}

void FloatingText::draw(sf::RenderWindow& window) {
	window.draw(text);
}