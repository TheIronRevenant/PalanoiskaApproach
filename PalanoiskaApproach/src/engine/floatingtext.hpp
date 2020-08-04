#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <string>
#include <SFML/Graphics.hpp>

class FloatingText {
public:
	FloatingText() { riseSpeed = 0.5f; riseDuration = 20; riseTimer = 0; }
	FloatingText(float x, float y, std::string text, const sf::Font& font);
	void update();
	void draw(sf::RenderWindow& window);
	bool isTerminated() const { return riseTimer >= riseDuration; }
private:
	sf::Text text;
	float riseSpeed;
	int riseDuration;
	int riseTimer;
};

#endif