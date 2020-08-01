#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "../engine/gameobject.hpp"

class Enemy : public GameObject {
public:
	Enemy() { currentHp = 10; }
	Enemy(unsigned int gridx, unsigned int gridy, sf::Texture& texture);
	void update();
	void draw(sf::RenderWindow& window);
private:
	int currentHp;
};

#endif