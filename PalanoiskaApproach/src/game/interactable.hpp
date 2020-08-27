#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "../engine/gameobject.hpp"
#include "dialogue.hpp"
#include <functional>

class Interactable : public GameObject {
public:
	Interactable() { enabled = true; }
	Interactable(unsigned int gridx, unsigned int y, sf::Texture& texture, DialogueInfo diagInfo);
	void update();
	void interact();
	void draw(sf::RenderWindow& window);
	std::function<void(DialogueInfo&)> interactfunc;
	DialogueInfo dialogueInfo;
	bool isEnabled() const { return enabled; }
	void enable() { enabled = true; }
	void disable() { enabled = false; }
private:
	bool enabled;
};

#endif