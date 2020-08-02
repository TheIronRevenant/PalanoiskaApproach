#include "enemy.hpp"
#include "../globals.hpp"
#include "playerattack.hpp"
#include "../engine/physics/collision.hpp"
#include <iostream>

Enemy::Enemy(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {
	currentHp = 30;
}

/*
Make enemies move
Deal damage
Player iframes
*/

void Enemy::update(std::vector<PlayerAttack>& attacks) {
	for (PlayerAttack& a : attacks) {
		if (isColliding(boundingBox, a.getBoundingBox())) {
			currentHp -= a.getDamage();
		}
	}
}

void Enemy::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}