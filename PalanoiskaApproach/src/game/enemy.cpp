#include "enemy.hpp"
#include "../globals.hpp"

Enemy::Enemy(unsigned int gridx, unsigned int gridy, sf::Texture& texture) : GameObject(gridx, gridy, texture) {
	currentHp = 10;
}

void Enemy::update() {

}

void Enemy::draw(sf::RenderWindow& window) {
	window.draw(sprite);

	if (Globals::Debugging) {
		window.draw(boundingBox);
	}
}