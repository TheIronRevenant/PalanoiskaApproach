#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "../engine/gameobject.hpp"
#include <functional>

class Interactable : public GameObject {
public:
	Interactable() {}
	Interactable(unsigned int gridx, unsigned int y, sf::Texture& texture);
	void update() {}
};

#endif