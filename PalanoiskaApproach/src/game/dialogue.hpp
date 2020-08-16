#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Dialogue {
public:
	Dialogue() {}
	Dialogue(float x, float y, std::string text, unsigned int fontSize, const sf::Font& font);
	void draw(sf::RenderWindow& window);
private:
	sf::Text text;
};

struct DialogueInfo {
	std::vector<Dialogue> dialogue;
	bool hasDialogue = false;
	bool visible = false;
	bool pauseGame = false;
	int diagCount = 0;
	int diagIndex = 0;
	int showTime = 0;
	int showCount = 0;
};

#endif