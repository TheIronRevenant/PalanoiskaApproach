#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "../engine/gameobject.hpp"
#include "dialogue.hpp"
#include <functional>

class Interactable : public GameObject {
public:
	Interactable() {}
	Interactable(unsigned int gridx, unsigned int y, sf::Texture& texture, DialogueInfo diagInfo);
	void update();
	void interact();
	void draw(sf::RenderWindow& window);
	std::function<void(DialogueInfo&)> interactfunc;
	DialogueInfo dialogueInfo;
};

#endif