#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <SFML/Graphics.hpp>
#include <string>

class Dialogue {
public:
	Dialogue() {}
	Dialogue(float x, float y, std::string text, unsigned int fontSize, const sf::Font& font);
	void draw(sf::RenderWindow& window);
private:
	sf::Text text;
};

struct DialogueInfo {
	Dialogue dialogue;
	bool hasDialogue = false;
	bool visible = false;
	bool pauseGame = false;
	int showTime = 0;
	int showCount = 0;
};

#endif