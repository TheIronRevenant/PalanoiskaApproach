#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../engine/gameobject.hpp"

class PlayerAttack;

class Enemy : public GameObject {
public:
	Enemy() { currentHp = 10; }
	Enemy(unsigned int gridx, unsigned int gridy, sf::Texture& texture);
	void update() {} //Override abstract
	void update(std::vector<PlayerAttack>& attacks);
	void draw(sf::RenderWindow& window);
	bool isDead() const { return currentHp <= 0; }
private:
	int currentHp;
};

#endif