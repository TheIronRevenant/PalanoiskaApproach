#include "uielement.hpp"
#include "../../globals.hpp"

UIElement::UIElement(float x, float y, std::function<void()> onClick, sf::Texture& texture) {
	this->onClick = onClick;

	clickBox = sf::FloatRect(x, y, (float)texture.getSize().x, (float)texture.getSize().y);

	sprite = sf::Sprite();
	sprite.setPosition(x, y);
	sprite.setTexture(texture);
}

void UIElement::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		sf::RectangleShape rect(sf::Vector2f(clickBox.width, clickBox.height));
		rect.setPosition(clickBox.left, clickBox.top);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Blue);
		rect.setOutlineThickness(0.5f);
		window.draw(rect);
	}
}

void UIElement::setWidth(float w) {
	sf::IntRect rect = sprite.getTextureRect();
	rect.width = (int)w;
	sprite.setTextureRect(rect);
}