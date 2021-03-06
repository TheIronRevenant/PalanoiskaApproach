#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <SFML/Graphics.hpp>
#include <functional>

class UIElement {
public:
	UIElement() {}
	UIElement(float x, float y, std::function<void()> onClick, sf::Texture& texture);
	std::function<void()> onClick; //Lambda function to determine on click behavior
	void draw(sf::RenderWindow& window);
	void setWidth(float w);
	const sf::FloatRect& getRect() const { return clickBox; }
private:
	sf::Sprite sprite;
	sf::FloatRect clickBox;
};

#endif